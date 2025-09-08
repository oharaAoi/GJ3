#include "BrightnessThreshold.h"
#include "Engine.h"
#include "Engine/Core/GraphicsContext.h"

BrightnessThreshold::~BrightnessThreshold() {
	bloomBuffer_->Finalize();
}

void BrightnessThreshold::Init() {
	GraphicsContext* graphicsCtx = GraphicsContext::GetInstance();
	// 輝度抽出の設定
	bloomBuffer_ = std::make_unique<DxResource>();
	bloomBuffer_->Init(graphicsCtx->GetDevice(), graphicsCtx->GetDxHeap(), ResourceType::COMMON);
	bloomBuffer_->CreateResource(sizeof(BloomSettings));
	bloomBuffer_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&bloomSetting_));

	bloomSetting_->threshold = 0.7f;

}

void BrightnessThreshold::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
	CopyData();
	Engine::SetPipeline(PSOType::ProcessedScene, "PostProcess_BrightnessThreshold.json");
	Pipeline* pso = Engine::GetLastUsedPipeline();
	UINT index = pso->GetRootSignatureIndex("gTexture");
	commandList->SetGraphicsRootDescriptorTable(index, pingResource->GetSRV().handleGPU);
	index = pso->GetRootSignatureIndex("gBloomSettings");
	commandList->SetGraphicsRootConstantBufferView(index, bloomBuffer_->GetResource()->GetGPUVirtualAddress());
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

void BrightnessThreshold::CheckBox() {
	ImGui::DragFloat("threshold##BrightnessThreshold", &bloomSetting_->threshold, 0.01f, 0.0f, 1.0f);
}

void BrightnessThreshold::Debug_Gui() {
	ImGui::DragFloat("threshold", &param_.threshold,0.001f,0.0f,0.0f,"%.3f");
	if (ImGui::Button("Save##BrightnessThreshold_save")) {
		param_.isEnable = isEnable_;
		JsonItems::Save("PostEffect", param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply##BrightnessThreshold_apply")) {
		param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
	}
}

void BrightnessThreshold::ApplySaveData() {
	param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
	isEnable_ = param_.isEnable;
	CopyData();
}

void BrightnessThreshold::CopyData() {
	bloomSetting_->threshold = param_.threshold;
}
