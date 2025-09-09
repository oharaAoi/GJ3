#include "StageSelectScene.h"
#include "Engine.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Engine/Module/PostEffect/PostProcess.h"
#include "Game/Manager/GhostEffectManager.h"

StageSelectScene::StageSelectScene(){}
StageSelectScene::~StageSelectScene(){ Finalize(); }

void StageSelectScene::Finalize(){
	PostProcess* postProcess = Engine::GetPostProcess();
	postProcess->SetIsActive(false);
	postProcess->GetBloom()->SetIsEnable(false);
	postProcess->GetVignette()->SetIsEnable(false);
	postProcess->GetToonMap()->SetIsEnable(false);

	sceneRenderer_->Finalize();
	GhostEffectManager::GetInstance()->Finalize();
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

	stageSelectCamera_ = std::make_unique<StageSelectCamera>();
	stageSelectCamera_->Init(camera2d_.get());

	// -------------------------------------------------
	// ↓ actorの初期化
	// -------------------------------------------------
	//worldObjects_ = std::make_unique<WorldObjects>();
	//worldObjects_->Init();

	// -------------------------------------------------
	// ↓ managerの初期化
	// -------------------------------------------------

	// -------------------------------------------------
	// ↓ spriteの初期化
	// -------------------------------------------------

	stageLoader_ = std::make_unique<StageLoader>();
	stageLoader_->Init();

	stageCollection = std::make_unique<StageSelectCollection>();
	stageCollection->Init(Engine::GetCanvas2d());

	stageContents_ = std::make_unique<StageContents>();
	stageContents_->Init(stageLoader_->GetMaxStageNum());
	stageContents_->SetStageSelectCollection(stageCollection.get());

	stageIndexUI_ = std::make_unique<StageIndexUI>();
	stageIndexUI_->Init("StageIndexUI");

	// -------------------------------------------------
	// ↓ behaviorの初期化
	// -------------------------------------------------
	behavior_ = std::make_unique<SelectingStageBehavior>(this);
	behavior_->Init();

	// -------------------------------------------------
	// ↓ 演出の初期化
	// ------------------------------------------------
	PostProcess* postProcess = Engine::GetPostProcess();
	postProcess->SetIsActive(true);

	auto bloom = postProcess->GetBloom();
	bloom->SetIsEnable(true);
	bloom->SetThreshold(0.14f);
	bloom->SetIntensity(0.25f);
	bloom->SetGaussianHeightTexelSizeFromFloat(0.f);
	bloom->SetGaussianWidthTexelSizeFromFloat(0.f);

	auto vignette = postProcess->GetVignette();
	vignette->SetIsEnable(true);
	vignette->SetColor({0.0235f,0.0235f,0.031f,1.f});
	vignette->SetScale(75.100f);
	vignette->SetPower(0.690f);

	postProcess->GetToonMap()->SetIsEnable(true);

	particle_ = ParticleManager::GetInstance()->CrateParticle("dust");
	particle_->Reset();

	gotRay_ = Engine::GetPostProcess()->GetEffect(PostEffectType::GOTRAY);
	gotRay_->ApplySaveData();

	// -------------------------------------------------
	// ↓ audioの初期化
	// ------------------------------------------------

	bgm_ = std::make_unique<AudioPlayer>();
	bgm_->Init("Select.mp3");
	bgm_->Play(true,0.5f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void StageSelectScene::Update(){

	// -------------------------------------------------
	// ↓ actorの更新
	// -------------------------------------------------
	//worldObjects_->Update();

	if(behavior_){
		behavior_->Update();
	}

	stageContents_->Update();
	stageCollection->Update(currentOffsetX_);

	// -------------------------------------------------
	// ↓ spriteの更新
	// -------------------------------------------------
	stageIndexUI_->Update(StageSelector::GetCurrentStageIndex());

	// -------------------------------------------------
	// ↓ cameraの更新
	// -------------------------------------------------
	if(debugCamera_->GetIsActive()){
		debugCamera_->Update();
	} else{
		camera3d_->Update();
	}
	camera2d_->Update();
	stageSelectCamera_->Update();

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
	stageSelector_->Init();
	stageSelector_->SetTotalStageNum(host_->stageLoader_->GetMaxStageNum());
	stageSelector_->SetStageSelectCollection(host_->stageCollection.get());
	stageSelector_->SetStageContents(host_->stageContents_.get());

	lightFlash_ = std::make_unique<LightFlash>();
	lightFlash_->Init("LightFlash");
}

void SelectingStageBehavior::Update(){
	lightFlash_->Update();

	stageSelector_->Update();
	host_->SetCurrenOffsetX(stageSelector_->GetCurrentOffsetX());

	if(stageSelector_->IsDecidedStage()){
		// ステージが決定したら次のシーンへ
		host_->ChangeBehavior(new TransitionToGameBehavior(host_));
		AudioPlayer::SinglShotPlay("start.mp3",0.3f);
	}
}

TransitionToGameBehavior::TransitionToGameBehavior(StageSelectScene* _host)
	:IStageSelectSceneBehavior(_host){}

TransitionToGameBehavior::~TransitionToGameBehavior(){}

void TransitionToGameBehavior::Init(){
	currentTime_ = 0.f;


	swirlTransition_ = std::make_unique<SwirlTransition>();
	swirlTransition_->Init();
	swirlTransition_->Close();
}

void TransitionToGameBehavior::Update(){
	currentTime_ += GameTimer::DeltaTime();
	if(swirlTransition_->GetIsFinish()){
		host_->SetNextSceneType(SceneType::GAME);
		return;
	}

	// effect Update ↓
	swirlTransition_->Update();
}

#pragma endregion // SelectingStageBehavior
