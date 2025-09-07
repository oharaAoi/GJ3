#include "Dissolve.h"
#include "Engine.h"
#include "Engine/Core/GraphicsContext.h"

Dissolve::~Dissolve() {
	settingBuffer_->Finalize();
}

void Dissolve::Init() {
	GraphicsContext* graphicsCtx = GraphicsContext::GetInstance();
	settingBuffer_ = std::make_unique<DxResource>();
	settingBuffer_->Init(graphicsCtx->GetDevice(), graphicsCtx->GetDxHeap(), ResourceType::COMMON);
	settingBuffer_->CreateResource(sizeof(DissolveSetting));
	settingBuffer_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&setting_));

	setting_->uvTransform = Matrix4x4::MakeUnit();
	setting_->color = Vector4(CVector3::UNIT, 1.0f);
	setting_->edgeColor = Vector4(CVector3::UNIT, 1.0f);
	setting_->threshold = 0.5f;
}

void Dissolve::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
	CopyData();

	Engine::SetPipeline(PSOType::ProcessedScene, "PostProcess_Dissolve.json");
	Pipeline* pso = Engine::GetLastUsedPipeline();
	UINT index = pso->GetRootSignatureIndex("gSetting");
	commandList->SetGraphicsRootConstantBufferView(index, settingBuffer_->GetResource()->GetGPUVirtualAddress());
	index = pso->GetRootSignatureIndex("gTexture");
	commandList->SetGraphicsRootDescriptorTable(index, pingResource->GetSRV().handleGPU);
	index = pso->GetRootSignatureIndex("gMaskTexture");
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, "noise0.png", index);
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

void Dissolve::CheckBox() {
	ImGui::Checkbox("Dissolve##Dissolve_checkbox", &isEnable_);
}

void Dissolve::Debug_Gui() {
	if (ImGui::CollapsingHeader("Dissolve##Dissolve_Header")) {
		ImGui::DragFloat("threshold", &param_.threshold, 0.01f);
		param_.threshold = std::clamp(param_.threshold, 0.0f, 1.0f);

		if (ImGui::Button("Save##Dissolve_save")) {
			param_.isEnable = isEnable_;
			JsonItems::Save("PostEffect", param_.ToJson(param_.GetName()));
		}
		if (ImGui::Button("Apply##Dissolve_apply")) {
			param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
		}
	}
}

void Dissolve::ApplySaveData() {
	param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
	isEnable_ = param_.isEnable;
	CopyData();
}

void Dissolve::CopyData() {
	setting_->color = param_.color;
	setting_->edgeColor = param_.edgeColor;
	setting_->threshold = param_.threshold;

	uvTransform_.scale = param_.uvTransform.scale;
	uvTransform_.rotate = param_.uvTransform.rotate;
	uvTransform_.translate = param_.uvTransform.translate;
}
