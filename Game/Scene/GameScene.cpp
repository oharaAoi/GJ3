#include "GameScene.h"
#include "Engine.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Game/Commands/ObjectCommandInvoker.h"
#include "Game/Input/StageInputHandler.h"

GameScene::GameScene(){}
GameScene::~GameScene(){ Finalize(); }

void GameScene::Finalize(){
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
	ghostSoulManager_->Init(Engine::GetCanvas2d(), player_.get());

	stageRegistry_ = std::make_unique<StageRegistry>();
	stageRegistry_->Init(Engine::GetCanvas2d());
	stageRegistry_->SetPlayer(player_.get());
	stageRegistry_->SetWindowSize({kWindowWidth_,kWindowHeight_});
	stageRegistry_->Register("stage_0.json");

	mapCollision_ = std::make_unique<MapCollisionSystem>();
	mapCollision_->Init(stageRegistry_.get(), ghostSoulManager_.get());

	worldObjects_ = std::make_unique<WorldObjects>();
	worldObjects_->Init();

	menuSelector_ = std::make_unique<MenuSelector>();
	menuSelector_->Init();

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

	// -------------------------------------------------
	// ↓ audioの初期化
	// ------------------------------------------------

	bgm_ = std::make_unique<AudioPlayer>();
	bgm_->Init("kinmokusei.mp3");
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

void GameScene::Update(){

	// -------------------------------------------------
	// ↓ actorの更新
	// -------------------------------------------------

	menuSelector_->Update();
	ChengeScene();
	// メニューを開いていなければ更新
	if (!menuSelector_->GetOpenMenu()) {
		player_->Update();
	}

	worldObjects_->Update();

	stageRegistry_->Update();

	mapCollision_->Update();

	ghostSoulManager_->Update();

	if(StageInputHandler::UndoInput()){
		ObjectCommandInvoker::GetInstance().UndoCommand();
		resetTimer_ = 0.f;
		AudioPlayer::SinglShotPlay("osii.mp3", 0.5f);
	} else if(StageInputHandler::RedoInput()){
		resetTimer_ = 0.f;
		ObjectCommandInvoker::GetInstance().RedoCommand();
		AudioPlayer::SinglShotPlay("osii.mp3", 0.5f);
	} else if(StageInputHandler::ResetInput()){
		resetTimer_ += GameTimer::DeltaTime();
		if(resetTimer_ >= kResetTime_){
			stageRegistry_->ResetStage();
			mapCollision_->ResetGhostCounter();
			ObjectCommandInvoker::GetInstance().ClearHistory();
			AudioPlayer::SinglShotPlay("osii.mp3", 0.5f);
		}
	} else{
		resetTimer_ = 0.f;
		// 特殊操作がないなら
		ObjectCommandInvoker::GetInstance().Update();
	}

	// クリア条件を満たしているかの判定
	if (!isClearConditionMet_) {
		if (stageRegistry_->GetNeedGhostNum() == mapCollision_->GetGhostCounter()) {
			AudioPlayer::SinglShotPlay("doragon.mp3", 0.5f);
			isClearConditionMet_ = true;
		}
	}

	// ステージをクリアしたかどうかの判定
	if (mapCollision_->GetIsClear()) {
		AudioPlayer::SinglShotPlay("fanfare.wav", 0.5f);
		nextSceneType_ = SceneType::STAGE_SELECT;
	}

	// -------------------------------------------------
	// ↓ spriteの更新
	// -------------------------------------------------

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

void GameScene::ChengeScene()
{
	if (menuSelector_->GetChengeScene()) {
		const auto type = menuSelector_->GetButtonType();
		switch (type)
		{
		case MenuButtonType::Select:
			// セレクトに戻る
			nextSceneType_ = SceneType::STAGE_SELECT;
			menuSelector_->SetChengeScene(false);
			break;
		case MenuButtonType::Reset :
			// ステージをリセットする
			stageRegistry_->ResetStage();
			menuSelector_->SetChengeScene(false);
			break;
		default:
			break;
		}
	}
}
