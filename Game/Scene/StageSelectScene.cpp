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

	// -------------------------------------------------
	// ↓ managerの初期化
	// -------------------------------------------------

	// -------------------------------------------------
	// ↓ spriteの初期化
	// -------------------------------------------------

	stageLoader_ = std::make_unique<StageLoader>();
	stageLoader_->Init();

	stageContents_ = std::make_unique<StageContents>();
	stageContents_->Init(stageLoader_->GetMaxStageNum());

	stageSelector_ = std::make_unique<StageSelector>();
	stageSelector_->SetStageRenderTarget(stageContents_->GetStageRenderTarget());
	stageSelector_->Init();
	stageSelector_->SetTotalStageNum(stageLoader_->GetMaxStageNum());

	// -------------------------------------------------
	// ↓ behaviorの初期化
	// -------------------------------------------------
	behavior_ = std::make_unique<SelectingStageBehavior>(this);
	behavior_->Init();

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
	bgm_->Play(true,0.5f);
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

	if(behavior_){
		behavior_->Update();
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

void StageSelectScene::ChangeBehavior(IStageSelectSceneBehavior* newBehavior){
	behavior_.reset(newBehavior);
	if(behavior_){
		behavior_->Init();
	}
}

#pragma region SelectingStageBehavior
SelectingStageBehavior::SelectingStageBehavior(StageSelectScene* _host)
	: IStageSelectSceneBehavior(_host){}
SelectingStageBehavior::~SelectingStageBehavior(){}

void SelectingStageBehavior::Init(){
	stageSelector_ = std::make_unique<StageSelector>();
	stageSelector_->SetStageRenderTarget(host_->stageContents_->GetStageRenderTarget());
	stageSelector_->Init();

	lightFlash_ = std::make_unique<LightFlash>();
	lightFlash_->Init("LightFlash");
}

void SelectingStageBehavior::Update(){
	/*if(lightFlash_->GetIsFinish()){
	}*/
	lightFlash_->Update();

	stageSelector_->Update();

	if(stageSelector_->IsDecidedStage()){
		// ステージが決定したら次のシーンへ
		// StageSelector::GetCurrentStageIndex(); // 現在のステージ番号を取得
		//host_->SetNextSceneType(SceneType::GAME);
		host_->ChangeBehavior(new TransitionToGameBehavior(host_));
		AudioPlayer::SinglShotPlay("fanfare.wav",0.3f);
	}
}

TransitionToGameBehavior::TransitionToGameBehavior(StageSelectScene* _host)
	:IStageSelectSceneBehavior(_host){}

TransitionToGameBehavior::~TransitionToGameBehavior(){}

void TransitionToGameBehavior::Init(){
	currentTime_ = 0.f;
}

void TransitionToGameBehavior::Update(){
	currentTime_ += GameTimer::DeltaTime();
	if(currentTime_ >= transitionTime_){
		host_->SetNextSceneType(SceneType::GAME);
		return;
	}

	// effect Update ↓

}

#pragma endregion // SelectingStageBehavior
