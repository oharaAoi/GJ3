#include "GameScene.h"
#include "Engine.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Game/Commands/ObjectCommandInvoker.h"
#include "Game/Input/StageInputHandler.h"

#include "Game/Manager/GhostEffectController.h"

#include "Game/UI/StageSelector.h"

std::optional<GameScene::Result> GameScene::s_lastResult_ = std::nullopt;

const std::optional<GameScene::Result>& GameScene::LastResult(){
	return s_lastResult_;
}
void GameScene::ClearLastResult(){
	s_lastResult_.reset();
}

GameScene::GameScene(){}
GameScene::~GameScene(){ Finalize(); }

void GameScene::Finalize(){
	GhostEffectController::GetInstance()->Init(stageRegistry_.get());

	sceneRenderer_->Finalize();
	ParticleManager::GetInstance()->Finalize();
	GpuParticleManager::GetInstance()->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Init(){
	JsonItems* adjust = JsonItems::GetInstance();
	adjust->Init("GameScene");

	// -------------------------------------------------
	// ↓ Sceneの初期化
	// -------------------------------------------------

	sceneRenderer_ = SceneRenderer::GetInstance();
	sceneRenderer_->Init();
	EditorWindows::GetInstance()->SetSceneRenderer(sceneRenderer_);

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
	player_->Init(Engine::GetCanvas2d());

	ghostSoulManager_ = std::make_unique<GhostSoulManager>();
	ghostSoulManager_->Init(Engine::GetCanvas2d(),player_.get());

	stageRegistry_ = std::make_unique<StageRegistry>();
	stageRegistry_->Init(Engine::GetCanvas2d());
	stageRegistry_->SetPlayer(player_.get());
	stageRegistry_->SetWindowSize({kWindowWidth_,kWindowHeight_});
	std::string loadName = "stage_" + std::to_string(StageSelector::GetCurrentStageIndex() + 1) + ".json";
	stageRegistry_->Register(loadName);

	mapCollision_ = std::make_unique<MapCollisionSystem>();
	mapCollision_->Init(stageRegistry_.get(),ghostSoulManager_.get());

	worldObjects_ = std::make_unique<WorldObjects>();
	worldObjects_->Init();

	menuSelector_ = std::make_unique<MenuSelector>();
	menuSelector_->Init();

	getGhostCountUI_ = std::make_unique<GetGhostCountUI>();
	getGhostCountUI_->Init(Engine::GetCanvas2d());

	stageResetUI_ = std::make_unique<StageResetUI>();
	stageResetUI_->SetTextureSize(stageRegistry_->GetTileSize());
	stageResetUI_->Init(Engine::GetCanvas2d());

	// -------------------------------------------------
	// ↓ managerの初期化
	// -------------------------------------------------
	ObjectCommandInvoker::GetInstance().Initialize();

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

	// -------------------------------------------------
	// ↓ audioの初期化
	// ------------------------------------------------

	bgm_ = std::make_unique<AudioPlayer>();
	bgm_->Init("Game.mp3");
	bgm_->Play(true,0.5f);

	// -------------------------------------------------
	// ↓ その他設定
	// -------------------------------------------------
	player_->SetTileSize(stageRegistry_->GetTileSize());
	player_->GetSprite()->ReSetTextureSize(stageRegistry_->GetTileSize());
	player_->SetIndex(stageRegistry_->GetStartIndex());
	player_->SetPosition(stageRegistry_->GetStartPos());
	player_->SetOffset(stageRegistry_->GetMapOffset());
	player_->SetMapCollision(mapCollision_.get());

	GhostEffectController::GetInstance()->Init(stageRegistry_.get());

	resetTimer_ = 0.f;
	isClearConditionMet_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Update(){

	// -------------------------------------------------
	// ↓ actorの更新
	// -------------------------------------------------

	menuSelector_->Update();
	ChengeScene();
	// メニューを開いていなければ更新
	if(!menuSelector_->GetOpenMenu()){
		player_->Update();
	}

	swirlTransition_->Update();

	worldObjects_->Update();

	stageRegistry_->Update();

	mapCollision_->Update();

	ghostSoulManager_->Update();

	stageResetUI_->Update();

	// Todo : Invoker を使う
	if(StageInputHandler::UndoInput()){
		ObjectCommandInvoker::GetInstance().UndoCommand();
		resetTimer_ = 0.f;
		AudioPlayer::SinglShotPlay("button.mp3",0.5f);
	} else if(StageInputHandler::RedoInput()){
		resetTimer_ = 0.f;
		ObjectCommandInvoker::GetInstance().RedoCommand();
		AudioPlayer::SinglShotPlay("button.mp3",0.5f);
	} else if(stageResetUI_->GetStageReset()){
		stageRegistry_->ResetStage();
		mapCollision_->ResetGhostCounter();
		ObjectCommandInvoker::GetInstance().ClearHistory();
		AudioPlayer::SinglShotPlay("button.mp3",0.5f);
		stageResetUI_->Reset();
		size_t size = ghostSoulManager_->GetSoulesSize();
		for(size_t i = 0; i < size; ++i){
			ghostSoulManager_->DeleteBackSoul();
		}
	} else{
		// 特殊操作がないなら
		ObjectCommandInvoker::GetInstance().Update();
	}

	getGhostCountUI_->Update(mapCollision_->GetGhostCounter(),stageRegistry_->GetNeedGhostNum());

	// クリア条件を満たしているかの判定
	if(!isClearConditionMet_){
		if(stageRegistry_->GetNeedGhostNum() == mapCollision_->GetGhostCounter()){
			AudioPlayer::SinglShotPlay("ghost_full.mp3",0.5f);
			isClearConditionMet_ = true;
		}
	}

	// ステージをクリアしたかどうかの判定
	if(mapCollision_->GetIsClear()){
		GameScene::Result r;
		r.ghostCount = mapCollision_->GetGhostCounter();
		GameScene::s_lastResult_ = r;
		AudioPlayer::SinglShotPlay("fanfare.wav",0.5f);
		nextSceneType_ = SceneType::CLEAR;
	}

	// -------------------------------------------------
	// ↓ spriteの更新
	// -------------------------------------------------
	GhostEffectController::GetInstance()->Update();

	// -------------------------------------------------
	// ↓ cameraの更新
	// -------------------------------------------------
	if(debugCamera_->GetIsActive()){
		debugCamera_->Update();
	} else{
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

void GameScene::Draw() const{
	// Sceneの描画
	sceneRenderer_->Draw();
}

void GameScene::ChengeScene(){
	if(menuSelector_->GetChengeScene()){
		const auto type = menuSelector_->GetButtonType();
		switch(type){
			case ButtonType::Select:
				// セレクトに戻る
				nextSceneType_ = SceneType::STAGE_SELECT;
				menuSelector_->SetChengeScene(false);
				break;
			case ButtonType::Reset:
				// ステージをリセットする
				stageRegistry_->ResetStage();
				menuSelector_->SetChengeScene(false);
				break;
			default:
				break;
		}
	}
}
