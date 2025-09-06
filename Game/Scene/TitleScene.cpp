#include "TitleScene.h"
#include "Engine.h"
#include "Engine/System/Input/Input.h"
#include "Engine/Lib/Json/JsonItems.h"

TitleScene::TitleScene(){}

TitleScene::~TitleScene(){
	Finalize();
}

void TitleScene::Finalize(){
	sceneRenderer_->Finalize();
	ParticleManager::GetInstance()->Finalize();
	GpuParticleManager::GetInstance()->Finalize();
}

void TitleScene::Init(){
	JsonItems* adjust = JsonItems::GetInstance();
	adjust->Init("TitleScene");

	sceneRenderer_ = SceneRenderer::GetInstance();
	sceneRenderer_->Init();
	EditorWindows::GetInstance()->SetSceneRenderer(sceneRenderer_);

	// -------------------------------------------------
	// ↓ cameraの初期化
	// -------------------------------------------------
	camera2d_ = std::make_unique<Camera2d>();
	camera3d_ = std::make_unique<Camera3d>();
	debugCamera_ = std::make_unique<DebugCamera>();

	camera2d_->Init();
	camera3d_->Init();
	debugCamera_->Init();

	// -------------------------------------------------
	// ↓ objectの初期化
	// -------------------------------------------------

	skybox_ = SceneRenderer::GetInstance()->AddObject<Skybox>("Skybox","Object_Skybox.json",-999);

	uis_ = std::make_unique<TitleUIs>();
	uis_->Init();

	thunderFlash_ = std::make_unique<ThunderFlash>();
	thunderFlash_->Init();

	lightFlash_ = std::make_unique<LightFlash>();
	lightFlash_->Init("LightFlash");

	DirectionalLight* light = Render::GetLightGroup()->GetDirectionalLight();
	light->SetIntensity(0.3f);

	// -------------------------------------------------
	// ↓ audioの初期化
	// ------------------------------------------------

	bgm_ = std::make_unique<AudioPlayer>();
	bgm_->Init("kinmokusei.mp3");
	bgm_->Play(true,0.5f);
}

void TitleScene::Update(){
	if(isTransition_){
		TransitionUpdate();
		if(lightFlash_->GetIsFinish()){
			isTransition_ = false;
			nextSceneType_ = SceneType::STAGE_SELECT;
		}
	} else{
		InputHandle();
		thunderFlash_->Update();
		Vector4 ghostColor = uis_->GetGhostSprite()->GetColor();
		float alpha = thunderFlash_->GetAlpha();
		ghostColor.w = (std::min)(alpha + (alpha * 1.1f),1.f);
		uis_->GetGhostSpriteRef()->SetColor(ghostColor);
	}

	// -------------------------------------------------
	// ↓ cameraの更新
	// -------------------------------------------------
	if(debugCamera_->GetIsActive()){
		debugCamera_->Update();
	} else{
		camera3d_->Update();
	}
	camera2d_->Update();

	uis_->Update();

	sceneRenderer_->Update();

	sceneRenderer_->PostUpdate();
}

void TitleScene::InputHandle(){
	Input* input = Input::GetInstance();
	bool doTransition = false;
	for(auto& key : kTransitionKeys){
		if(Input::IsTriggerKey(key)){
			doTransition = true;
			break;
		}
	}
	if(!doTransition){
		if(input->IsControllerConnected()){
			for(auto& button : kTransitionButtons){
				if(Input::IsTriggerButton(button)){
					doTransition = true;
					break;
				}
			}
		}
	}

	if(doTransition){
		isTransition_ = true;
		thunderFlash_->SetFlashColor(Vector4(0.f,0.f,0.f,0.f));
		lightFlash_->SetFlashColor(Vector4(0.f,0.f,0.f,0.f));
		AudioPlayer::SinglShotPlay("fanfare.wav",0.3f);
	}
}

void TitleScene::TransitionUpdate(){
	lightFlash_->Update();
	Vector4 ghostColor = uis_->GetGhostSprite()->GetColor();
	ghostColor.w = 1.f - lightFlash_->GetFlashColor().w;
	uis_->GetGhostSpriteRef()->SetColor(ghostColor);
}

void TitleScene::Draw() const{

	sceneRenderer_->Draw();
}
