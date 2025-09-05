#include "DistortionEffect.h"
#include "Engine.h"
#include "Engine/Core/GraphicsContext.h"
#include "Engine/System/Manager/TextureManager.h"

DistortionEffect::~DistortionEffect() {
	settingBuffer_->Finalize();
}

void DistortionEffect::Init() {
	GraphicsContext* graphicsCtx = GraphicsContext::GetInstance();
	settingBuffer_ = std::make_unique<DxResource>();
	settingBuffer_->Init(graphicsCtx->GetDevice(), graphicsCtx->GetDxHeap(), ResourceType::COMMON);
	settingBuffer_->CreateResource(sizeof(DistortionEffectSetting));
	settingBuffer_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&setting_));

	setting_->uv = Matrix4x4::MakeUnit();
	setting_->bias = 0.5f;
	setting_->strength = 1.0f;

	uvTransform_ = SRT();
	noiseTextureName_ = "noise0.png";
}

void DistortionEffect::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
	CopyData();
	setting_->uv = uvTransform_.MakeAffine();

	Pipeline* pso = Engine::SetPipeline(PSOType::ProcessedScene, "PostProcess_Distortion.json");
	UINT index = pso->GetRootSignatureIndex("gDistortionTexture");
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, noiseTextureName_, index);
	index = pso->GetRootSignatureIndex("gSceneViewTexture");
	commandList->SetGraphicsRootDescriptorTable(index, pingResource->GetSRV().handleGPU);
	index = pso->GetRootSignatureIndex("gEffectParam");
	commandList->SetGraphicsRootConstantBufferView(index, settingBuffer_->GetResource()->GetGPUVirtualAddress());
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

void DistortionEffect::CheckBox() {
	ImGui::Checkbox("Distortion##Distortion_checkBox", &isEnable_);
}

void DistortionEffect::Debug_Gui() {
	if (ImGui::CollapsingHeader("Distortion##Distortion_checkBox")) {
		ImGui::DragFloat3("scale", &param_.uvTransform.scale.x, 0.1f);
		ImGui::DragFloat3("rotate", &param_.uvTransform.rotate.x, 0.1f);
		ImGui::DragFloat3("translate", &param_.uvTransform.translate.x, 0.1f);
		ImGui::DragFloat("bias", &param_.bias);
		ImGui::DragFloat("strength", &param_.strength);
		TextureManager* textureManager = TextureManager::GetInstance();
		noiseTextureName_ = textureManager->SelectTexture(noiseTextureName_);

		if (ImGui::Button("Save")) {
			param_.isEnable = isEnable_;
			JsonItems::Save("PostEffect", param_.ToJson(param_.GetName()));
		}
		if (ImGui::Button("Apply")) {
			param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
		}
	}
}

void DistortionEffect::ApplySaveData() {
	param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
	isEnable_ = param_.isEnable;
	CopyData();
}

void DistortionEffect::CopyData() {
	uvTransform_.scale = param_.uvTransform.scale;
	uvTransform_.rotate = param_.uvTransform.rotate;
	uvTransform_.translate = param_.uvTransform.translate;
	setting_->bias = param_.bias;
	setting_->strength = param_.strength;
}
