#include "StageSelectScene.h"
#include "Engine.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Engine/Module/PostEffect/PostProcess.h"

StageSelectScene::StageSelectScene(){}
StageSelectScene::~StageSelectScene(){ Finalize(); }

void StageSelectScene::Finalize(){
	sceneRenderer_->Finalize();
	ParticleManager::GetInstance()->Finalize();
	GpuParticleManager::GetInstance()->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void StageSelectScene::Init(){
	JsonItems* adjust = JsonItems::GetInstance();
	adjust->Init("StageSelectScene");

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
	worldObjects_ = std::make_unique<WorldObjects>();
	worldObjects_->Init();

	stageContents_ = std::make_unique<StageContents>();
	stageContents_->Init();

	// -------------------------------------------------
	// ↓ managerの初期化
	// -------------------------------------------------

	// -------------------------------------------------
	// ↓ spriteの初期化
	// -------------------------------------------------
	stageSelector_ = std::make_unique<StageSelector>();
	stageSelector_->SetStageRenderTarget(stageContents_->GetStageRenderTarget());
	stageSelector_->Init();

	// -------------------------------------------------
	// ↓ 演出の初期化
	// ------------------------------------------------

	particle_ = ParticleManager::GetInstance()->CrateParticle("dust");
	particle_->Reset();

	gotRay_ = Engine::GetPostProcess()->GetEffect(PostEffectType::GOTRAY);
	gotRay_->ApplySaveData();

	// -------------------------------------------------
	// ↓ audioの初期化
	// ------------------------------------------------

	bgm_ = std::make_unique<AudioPlayer>();
	bgm_->Init("kinmokusei.mp3");
	bgm_->Play(true, 0.5f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void StageSelectScene::Update(){

	stageContents_->Update();

	// -------------------------------------------------
	// ↓ actorの更新
	// -------------------------------------------------
	worldObjects_->Update();
	stageSelector_->Update();
	if(stageSelector_->IsDecidedStage()){
		// ステージが決定したら次のシーンへ
		// StageSelector::GetCurrentStageIndex(); // 現在のステージ番号を取得
		nextSceneType_ = SceneType::GAME;
		AudioPlayer::SinglShotPlay("fanfare.wav", 0.3f);
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
	// ↓ あたり判定
	// -------------------------------------------------

	// -------------------------------------------------
	// ↓ 最後に行いたい更新
	// -------------------------------------------------

	sceneRenderer_->PostUpdate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void StageSelectScene::Draw() const{
	// Sceneの描画
	sceneRenderer_->Draw();
}
