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
	settingBuffer_->CreateResource(sizeof(DistortionEffectParam));
	settingBuffer_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&setting_));

	setting_->uv = Matrix4x4::MakeUnit();
	setting_->bias = 0.5f;
	setting_->strength = 1.0f;

	uvTransform_ = SRT();
	noiseTextureName_ = "noise0.png";
}

void DistortionEffect::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
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
		ImGui::DragFloat3("scale", &uvTransform_.scale.x, 0.1f);
		ImGui::DragFloat3("rotate", &uvTransform_.rotate.x, 0.1f);
		ImGui::DragFloat3("translate", &uvTransform_.translate.x, 0.1f);
		ImGui::DragFloat("bias", &setting_->bias);
		ImGui::DragFloat("strength", &setting_->strength);
		TextureManager* textureManager = TextureManager::GetInstance();
		noiseTextureName_ = textureManager->SelectTexture(noiseTextureName_);
	}
}
