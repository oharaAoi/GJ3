#include "GameScene.h"
#include "Engine.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Game/Commands/ObjectCommandInvoker.h"
#include "Game/Input/StageInputHandler.h"
#include "Game/UI/StageSelect/StageSelector.h"

std::optional<GameScene::Result> GameScene::s_lastResult_ = std::nullopt;

const std::optional<GameScene::Result> &GameScene::LastResult()
{
	return s_lastResult_;
}
void GameScene::ClearLastResult()
{
	s_lastResult_.reset();
}

GameScene::GameScene() {}
GameScene::~GameScene() { Finalize(); }

void GameScene::Finalize()
{
	Engine::GetPostProcess()->SetAllEnable(false);

	sceneRenderer_->Finalize();
	ghostEffectManager_->Finalize();
	limitBlockEffectManager_->Finalize();
	ParticleManager::GetInstance()->Finalize();
	GpuParticleManager::GetInstance()->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Init()
{
	JsonItems *adjust = JsonItems::GetInstance();
	adjust->Init("GameScene");

	// -------------------------------------------------
	// ↓ Sceneの初期化
	// -------------------------------------------------

	sceneRenderer_ = SceneRenderer::GetInstance();
	sceneRenderer_->Init();
	EditorWindows::GetInstance()->SetSceneRenderer(sceneRenderer_);

	ghostEffectManager_ = GhostSmokeManager::GetInstance();
	ghostEffectManager_->Init();

	limitBlockEffectManager_ = LimitBlockEffectManager::GetInstance();
	limitBlockEffectManager_->Init();

	// -------------------------------------------------
	// ↓ cameraの初期化
	// -------------------------------------------------

	camera3d_ = std::make_unique<Camera3d>();
	debugCamera_ = std::make_unique<DebugCamera>();
	camera2d_ = std::make_unique<Camera2d>();
	camera3d_->Init();
	debugCamera_->Init();
	camera2d_->Init();

	// -------------------------------------------------
	// ↓ actorの初期化
	// -------------------------------------------------

	player_ = std::make_unique<Player>();
	player_->Init(Engine::GetCanvas2d(), {0, 0}, {0, 0});

	ghostSoulManager_ = std::make_unique<GhostSoulManager>();
	ghostSoulManager_->Init(Engine::GetCanvas2d(), player_.get());

	stageRegistry_ = std::make_unique<StageRegistry>();
	stageRegistry_->Init(Engine::GetCanvas2d());
	stageRegistry_->SetPlayer(player_.get());
	stageRegistry_->SetWindowSize({kWindowWidth_, kWindowHeight_});
	std::string loadName = "stage_" + std::to_string(StageSelector::GetCurrentStageIndex() + 1) + ".json";
	stageRegistry_->Register(loadName);

	mapCollision_ = std::make_unique<MapCollisionSystem>();
	mapCollision_->Init(stageRegistry_.get(), ghostSoulManager_.get());

	worldObjects_ = std::make_unique<WorldObjects>();
	worldObjects_->Init();

	menuSelector_ = std::make_unique<MenuSelector>();
	menuSelector_->Init(Engine::GetCanvas2d());

	getGhostCountUI_ = std::make_unique<GetGhostCountUI>();
	getGhostCountUI_->Init(Engine::GetCanvas2d());

	stageResetUI_ = std::make_unique<StageResetUI>();
	stageResetUI_->SetTextureSize(stageRegistry_->GetTileSize());
	stageResetUI_->Init(Engine::GetCanvas2d());
	if (StageSelector::GetCurrentStageIndex() == 0)
	{
		tutorialDirector_ = std::make_unique<TutorialDirector>();
		tutorialDirector_->Init();
	}

	// -------------------------------------------------
	// ↓ managerの初期化
	// -------------------------------------------------
	ObjectCommandInvoker::GetInstance().Initialize();

	// -------------------------------------------------
	// ↓ PostEffect の初期化
	// -------------------------------------------------
	PostProcess *postProcess = Engine::GetPostProcess();
	postProcess->SetIsActive(true);
	postProcess->GetToonMap()->SetIsEnable(true);
	postProcess->GetBloom()->SetEnable(true);
	postProcess->GetBloom()->ApplySaveData();
	postProcess->GetVignette()->SetIsEnable(true);
	postProcess->GetVignette()->ApplySaveData();

	// -------------------------------------------------
	// ↓ spriteの初期化
	// ------------------------------------------------

	orb_ = ParticleManager::GetInstance()->CrateParticle("orb");
	orb_->Reset();

	dust_ = ParticleManager::GetInstance()->CrateParticle("dust");
	dust_->Reset();

	swirlTransition_ = std::make_unique<SwirlTransition>();
	swirlTransition_->Init();
	swirlTransition_->Open();
	AudioPlayer::SinglShotPlay("start.mp3", 0.3f);

	gameUIs_ = std::make_unique<GameUIs>();
	gameUIs_->Init();

	// -------------------------------------------------
	// ↓ audioの初期化
	// ------------------------------------------------

	bgm_ = std::make_unique<AudioPlayer>();
	bgm_->Init("Game.mp3");
	bgm_->Play(true, 0.5f);

	// -------------------------------------------------
	// ↓ その他設定
	// -------------------------------------------------
	player_->SetTileSize(stageRegistry_->GetTileSize());
	player_->GetSprite()->ReSetTextureSize(stageRegistry_->GetTileSize());
	player_->SetIndex(stageRegistry_->GetStartIndex());
	player_->SetPosition(stageRegistry_->GetStartPos());
	player_->SetOffset(stageRegistry_->GetMapOffset());
	player_->SetMapCollision(mapCollision_.get());

	resetTimer_ = 0.f;
	isClearConditionMet_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Update()
{

	// -------------------------------------------------
	// ↓ actorの更新
	// -------------------------------------------------
	if (tutorialDirector_ != nullptr && !menuSelector_->GetOpenMenu())
	{
		tutorialDirector_->SetGhostCount(mapCollision_->GetGhostCounter());
		tutorialDirector_->SetCreateGhost(mapCollision_->GetCreateGhost());
		tutorialDirector_->Update();
	}

	bool isTutorial = tutorialDirector_ == nullptr || tutorialDirector_->GetIsMoveEnable();
	if (isTutorial && !mapCollision_->GetIsClear())
	{
		menuSelector_->Update();
		ChengeScene();
	}
	// メニューを開いていなければ更新
	if (!menuSelector_->GetOpenMenu() && isTutorial && !mapCollision_->GetIsClear())
	{
		player_->Update();
	}
	ghostEffectManager_->Update();
	limitBlockEffectManager_->Update();

	swirlTransition_->Update();

	worldObjects_->Update();

	stageRegistry_->Update();

	mapCollision_->Update();

	ghostSoulManager_->Update();

	if (behavior_)
	{
		behavior_->Update();
	}

	bool isEnable = !menuSelector_->GetOpenMenu() && isTutorial;
	if (isEnable && !mapCollision_->GetIsClear())
	{
		stageResetUI_->Update();
	}

	if (!menuSelector_->GetOpenMenu())
	{

		// UI Input
		bool padIsInput = false;
		bool keyIsInput = false;

		// Command
		UndoRedoState state = ObjectCommandInvoker::GetInstance().InputHandle(padIsInput, keyIsInput);

		padIsInput |= stageResetUI_->GetPadInput();
		keyIsInput |= stageResetUI_->GetKeyInput();

		padIsInput |= player_->isPadInput();
		keyIsInput |= player_->isKeyInput();

		if (state == UndoRedoState::UNDO)
		{
			ObjectCommandInvoker::GetInstance().UndoCommand();
			resetTimer_ = 0.f;
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
		}
		else if (state == UndoRedoState::REDO)
		{
			resetTimer_ = 0.f;
			ObjectCommandInvoker::GetInstance().RedoCommand();
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
		}
		else
		{
			// 特殊操作がないなら
			ObjectCommandInvoker::GetInstance().ExecuteCommandRequest();
		}

		if (stageResetUI_->GetStageReset())
		{
			ghostEffectManager_->Finalize();
			limitBlockEffectManager_->Finalize();

			stageRegistry_->ResetStage();
			mapCollision_->ResetGhostCounter();
			ObjectCommandInvoker::GetInstance().ClearHistory();
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			stageResetUI_->Reset();
			size_t size = ghostSoulManager_->GetSoulesSize();
			for (size_t i = 0; i < size; ++i)
			{
				ghostSoulManager_->DeleteBackSoul();
			}
		}
		gameUIs_->Update(keyIsInput, padIsInput);
	}

	getGhostCountUI_->Update(mapCollision_->GetGhostCounter(), stageRegistry_->GetNeedGhostNum());

	// クリア条件を満たしているかの判定
	if (!isClearConditionMet_)
	{
		if (stageRegistry_->GetNeedGhostNum() == mapCollision_->GetGhostCounter())
		{
			AudioPlayer::SinglShotPlay("ghost_full.mp3", 0.5f);
			isClearConditionMet_ = true;
		}
	}

	// ステージをクリアしたかどうかの判定
	if (mapCollision_->GetIsClear())
	{
		if (clearTimer_ == 0.0f)
		{
			AudioPlayer::SinglShotPlay("goal.mp3", 0.5f);
			AudioPlayer::SinglShotPlay("start.mp3", 0.3f);
			swirlTransition_->Close();
		}
		clearTimer_ += GameTimer::DeltaTime();
		if (swirlTransition_->GetIsFinish())
		{
			GameScene::Result r;
			r.ghostCount = mapCollision_->GetGhostCounter();
			GameScene::s_lastResult_ = r;
			AudioPlayer::SinglShotPlay("fanfare.wav", 0.5f);
			nextSceneType_ = SceneType::CLEAR;
		}
	}

	// -------------------------------------------------
	// ↓ spriteの更新
	// -------------------------------------------------

	// -------------------------------------------------
	// ↓ cameraの更新
	// -------------------------------------------------
	if (debugCamera_->GetIsActive())
	{
		debugCamera_->Update();
	}
	else
	{
		camera3d_->Update();
	}
	camera2d_->Update();

	// -------------------------------------------------
	// ↓ sceneの更新
	// -------------------------------------------------
	sceneRenderer_->Update();

	// -------------------------------------------------
	// ↓ 最後に行いたい更新
	// -------------------------------------------------

	sceneRenderer_->PostUpdate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Draw() const
{
	// Sceneの描画
	sceneRenderer_->Draw();

	ghostEffectManager_->Draw();
	limitBlockEffectManager_->Draw();
}

void GameScene::ChengeScene()
{
	if (menuSelector_->GetChengeScene())
	{
		const auto type = menuSelector_->GetButtonType();
		switch (type)
		{
		case ButtonType::Select:
			// セレクトに戻る
			nextSceneType_ = SceneType::STAGE_SELECT;
			menuSelector_->SetChengeScene(false);
			break;
		case ButtonType::Reset:
		{
			// ステージをリセットする
			ghostEffectManager_->Finalize();
			limitBlockEffectManager_->Finalize();

			stageRegistry_->ResetStage();
			if (tutorialDirector_ != nullptr)
			{
				tutorialDirector_->TutorialReset();
			}
			mapCollision_->ResetGhostCounter();
			stageResetUI_->Reset();
			size_t size = ghostSoulManager_->GetSoulesSize();
			for (size_t i = 0; i < size; ++i)
			{
				ghostSoulManager_->DeleteBackSoul();
			}
			menuSelector_->SetChengeScene(false);
			menuSelector_->SetOpenMenu(false);
			swirlTransition_->Open();
			AudioPlayer::SinglShotPlay("start.mp3", 0.3f);
		}
		break;
		default:
			break;
		}
	}
	if (menuSelector_->GetChangeEffect())
	{
		if (menuSelector_->GetSelectButton())
		{
			menuSelector_->SetChangeEffect(false);
			ChangeBehavior(std::make_unique<ChangeSelectSceneBehavior>(this));
		}
		else if (menuSelector_->GetResetButton())
		{
			swirlTransition_->Close();
			AudioPlayer::SinglShotPlay("start.mp3", 0.3f);
			menuSelector_->SetChangeEffect(false);
		}
	}
}

void GameScene::ChangeBehavior(std::unique_ptr<IGameSceneBehavior> newBehavior)
{
	behavior_ = std::move(newBehavior);
	behavior_->Init();
}

ChangeSelectSceneBehavior::ChangeSelectSceneBehavior(GameScene *_host) : IGameSceneBehavior(_host) {}
ChangeSelectSceneBehavior::~ChangeSelectSceneBehavior() {}

void ChangeSelectSceneBehavior::Init()
{
	lightFlash_ = std::make_unique<LightFlash>();
	lightFlash_->Init("LightFlash");
}

void ChangeSelectSceneBehavior::Update()
{
	lightFlash_->Update();
}
