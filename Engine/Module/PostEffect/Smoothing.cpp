#include "Smoothing.h"
#include "Engine.h"

Smoothing::~Smoothing() {
	settingBuffer_->Finalize();
}

void Smoothing::Init() {
	GraphicsContext* graphicsCtx = GraphicsContext::GetInstance();
	settingBuffer_ = std::make_unique<DxResource>();
	settingBuffer_->Init(graphicsCtx->GetDevice(), graphicsCtx->GetDxHeap(), ResourceType::COMMON);
	settingBuffer_->CreateResource(sizeof(Setting));
	settingBuffer_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&setting_));

	setting_->size = 5;
}

void Smoothing::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
	CopyData();

	Engine::SetPipeline(PSOType::ProcessedScene, "PostProcess_Smoothing.json");
	Pipeline* pso = Engine::GetLastUsedPipeline();
	UINT index = pso->GetRootSignatureIndex("gTexture");
	commandList->SetGraphicsRootDescriptorTable(index, pingResource->GetSRV().handleGPU);
	index = pso->GetRootSignatureIndex("gSmoothSetting");
	commandList->SetGraphicsRootConstantBufferView(index, settingBuffer_->GetResource()->GetGPUVirtualAddress());
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

void Smoothing::CheckBox() {
	ImGui::Checkbox("Smoothing##Smoothing_checkbox", &isEnable_);
}

void Smoothing::Debug_Gui() {
	if (ImGui::CollapsingHeader("Smoothing")) {
		ImGui::DragScalar("size", ImGuiDataType_U32, &param_.size);

		if (ImGui::Button("Save##smoothing_save")) {
			param_.isEnable = isEnable_;
			JsonItems::Save("PostEffect", param_.ToJson(param_.GetName()));
		}
		if (ImGui::Button("Apply##smoothing_apply")) {
			param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
		}
	}
}

void Smoothing::ApplySaveData() {
	param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
	isEnable_ = param_.isEnable;
	CopyData();
}

void Smoothing::CopyData() {
	setting_->size = param_.size;
}
