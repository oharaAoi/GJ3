#include "TitleScene.h"
#include "Engine.h"
#include "Engine/System/Input/Input.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Game/Manager/GhostSmokeManager.h"

#include "Engine.h"

#include "Engine/Lib/Math/Easing.h"

TitleScene::TitleScene(){}

TitleScene::~TitleScene(){
	Finalize();
}

void TitleScene::Finalize(){
	PostProcess* postProcess = Engine::GetPostProcess();
	postProcess->SetIsActive(false);
	postProcess->GetBloom()->SetIsEnable(false);
	postProcess->GetVignette()->SetIsEnable(false);
	postProcess->GetToonMap()->SetIsEnable(false);

	sceneRenderer_->Finalize();
	GhostSmokeManager::GetInstance()->Finalize();
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
	// ↓ PostProcess の初期化
	// -------------------------------------------------
	PostProcess* postProcess = Engine::GetPostProcess();
	postProcess->SetIsActive(true);

	postProcess->GetGotRay()->SetIsEnable(false);

	postProcess->GetBloom()->SetIsEnable(true);
	postProcess->GetVignette()->SetIsEnable(true);
	postProcess->GetVignette()->SetColor(Vector4(0.0222f,0.0222f,0.0355f,1.f));
	postProcess->GetToonMap()->SetIsEnable(true);

	// params
	minParam_ = std::make_unique<PostEffectParam>();
	maxParam_ = std::make_unique<PostEffectParam>();
	minParam_->Init("TitlePostEffectMinParam");
	maxParam_->Init("TitlePostEffectMaxParam");

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
	bgm_->Init("Title.mp3");
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
		// オバケの色を 変える
		Vector4 ghostColor = uis_->GetGhostSprite()->GetColor();
		float alpha = thunderFlash_->GetAlpha();
		ghostColor.w = (std::min)(alpha + (alpha * 1.1f),1.f);
		uis_->GetGhostSpriteRef()->SetColor(ghostColor);

		PostProcess* postProcess = Engine::GetPostProcess();
		
		// bloom
		auto bloom = postProcess->GetBloom();
		float bloomT = Ease::Out::Quad(alpha);
		bloom->SetThreshold(std::lerp(minParam_->bloomThreshold_,maxParam_->bloomThreshold_,bloomT));
		bloom->SetIntensity(std::lerp(minParam_->bloomIntensity_,maxParam_->bloomIntensity_,bloomT));
		float texelSize = std::lerp(minParam_->bloomTexelSize_,maxParam_->bloomTexelSize_,bloomT);
		bloom->SetGaussianWidthTexelSizeFromFloat(texelSize);
		bloom->SetGaussianHeightTexelSizeFromFloat(texelSize);

		// vignette
		auto vignette = postProcess->GetVignette();
		vignette->SetPower(std::lerp(minParam_->vignettePower_,maxParam_->vignettePower_,Ease::Out::Expo(alpha)));
		vignette->SetScale(std::lerp(minParam_->vignetteScale_,maxParam_->vignetteScale_,Ease::Out::Expo(alpha)));
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
		lightFlash_->SetFlashColor(Vector4(0.f,0.f,0.f,0.f));
		AudioPlayer::SinglShotPlay("button.mp3",0.3f);
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

#pragma region PostEffectParam

void TitleScene::PostEffectParam::Init(const std::string& _paramName){
	IJsonConverter::SetName(_paramName);
	AttributeGui::SetName(_paramName);

	// 読み込み
	this->FromJson(JsonItems::GetData("Effect",_paramName));

	EditorWindows::AddObjectWindow(this,_paramName);
}

void TitleScene::PostEffectParam::Debug_Gui(){
		ImGui::DragFloat("Bloom Intensity",&bloomIntensity_,0.01f,0.f);
		ImGui::DragFloat("Bloom Threshold",&bloomThreshold_,0.01f,0.f);
		ImGui::DragFloat("Bloom Texel Size",&bloomTexelSize_,0.01f,0.1f);
		ImGui::DragFloat("Vignette Power",&vignettePower_,0.01f,0.f);
		ImGui::DragFloat("Vignette Scale",&vignetteScale_,0.01f,0.f);

		if(ImGui::Button("Save##TitleScenePostEffectParam")){
			JsonItems::Save("Effect",ToJson(IJsonConverter::GetName()));
		}
}

json TitleScene::PostEffectParam::ToJson(const std::string& id) const{
	JsonBuilder builder(id);
	builder.Add("bloomIntensity",bloomIntensity_);
	builder.Add("bloomThreshold",bloomThreshold_);
	builder.Add("bloomTexelSize",bloomTexelSize_);
	builder.Add("vignettePower",vignettePower_);
	builder.Add("vignetteScale",vignetteScale_);

	return builder.Build();
}

void TitleScene::PostEffectParam::FromJson(const json& jsonData){
	fromJson(jsonData,"bloomIntensity",bloomIntensity_);
	fromJson(jsonData,"bloomThreshold",bloomThreshold_);
	fromJson(jsonData,"bloomTexelSize",bloomTexelSize_);
	fromJson(jsonData,"vignettePower",vignettePower_);
	fromJson(jsonData,"vignetteScale",vignetteScale_);
}

#pragma endregion