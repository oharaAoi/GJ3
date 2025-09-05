#include "GaussianBlurHeight.h"
#include "Engine.h"
#include "Engine/Core/GraphicsContext.h"

GaussianBlurHeight::~GaussianBlurHeight() {
	blurBuffer_->Finalize();
}

void GaussianBlurHeight::Init() {
	GraphicsContext* graphicsCtx = GraphicsContext::GetInstance();
	// blurの設定
	blurBuffer_ = std::make_unique<DxResource>();
	blurBuffer_->Init(graphicsCtx->GetDevice(), graphicsCtx->GetDxHeap(), ResourceType::COMMON);
	blurBuffer_->CreateResource(sizeof(BlurSettings));
	blurBuffer_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&blurSetting_));

	blurSetting_->texelSize = { 1.0f / (float)kWindowWidth_, 1.0f / (float)kWindowHeight_ };
}

void GaussianBlurHeight::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
	CopyData();
	// blur
	Engine::SetPipeline(PSOType::ProcessedScene, "PostProcess_GaussianBlurHight.json");
	Pipeline* pso = Engine::GetLastUsedPipeline();
	UINT index = pso->GetRootSignatureIndex("gTexture");
	commandList->SetGraphicsRootDescriptorTable(index, pingResource->GetSRV().handleGPU);
	index = pso->GetRootSignatureIndex("gBlurSettings");
	commandList->SetGraphicsRootConstantBufferView(index, blurBuffer_->GetResource()->GetGPUVirtualAddress());
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);

}

void GaussianBlurHeight::CheckBox() {
	ImGui::Checkbox("GaussianBlurHeight", &isEnable_);
}

void GaussianBlurHeight::Debug_Gui() {
	static float sample = 1;
	ImGui::DragFloat("sampleHeight", &sample, 0.1f, 0.0f, 10.0f);
	param_.texelSize = { sample / (float)kWindowWidth_, sample / (float)kWindowHeight_ };

	if (ImGui::Button("Save")) {
		JsonItems::Save("PostEffect", param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
	}
}

void GaussianBlurHeight::ApplySaveData() {
	param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
	CopyData();
}

void GaussianBlurHeight::CopyData() {
	blurSetting_->texelSize = param_.texelSize;
}
