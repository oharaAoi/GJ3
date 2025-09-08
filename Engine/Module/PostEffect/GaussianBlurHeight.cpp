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
	ImGui::Checkbox("GaussianBlurHeight##Gausianfiler_checkbox", &isEnable_);
}

void GaussianBlurHeight::Debug_Gui() {
	ImGui::DragFloat2("sampleHeight", &param_.texelSize.x,0.01f,0.0f,0.0f,"%.4f");
	
	if (ImGui::Button("Save##Gaussian_save")) {
		JsonItems::Save("PostEffect", param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply##Gaussian_apply")) {
		param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
	}
}

void GaussianBlurHeight::ApplySaveData() {
	param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
	CopyData();
}

void GaussianBlurHeight::CopyData() {
	blurSetting_->texelSize = { param_.texelSize.x / (float)kWindowWidth_, param_.texelSize.y / (float)kWindowHeight_ };
}
