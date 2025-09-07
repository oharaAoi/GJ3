#include "SwirlEffect.h"
#include "Engine.h"
#include "Engine/Core/GraphicsContext.h"
#include "Engine/System/Manager/TextureManager.h"

SwirlEffect::~SwirlEffect() {
	settingBuffer_->Finalize();
}

void SwirlEffect::Init() {
	GraphicsContext* graphicsCtx = GraphicsContext::GetInstance();
	settingBuffer_ = std::make_unique<DxResource>();
	settingBuffer_->Init(graphicsCtx->GetDevice(), graphicsCtx->GetDxHeap(), ResourceType::COMMON);
	settingBuffer_->CreateResource(sizeof(SwirlSetting));
	settingBuffer_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&setting_));

	setting_->uv = Matrix4x4::MakeUnit();

	param_.uvTransform.scale = CVector3::UNIT;
	param_.uvTransform.rotate = CVector3::ZERO;
	param_.uvTransform.translate = CVector3::ZERO;


	swirlMask_ = std::make_unique<SwirlMask>();
	swirlMask_->Init();

	param_.patternTextureName = "noise0.png";
	isEnable_ = true;
}

void SwirlEffect::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
	
	swirlMask_->SetCommand(commandList, pingResource);
	postProcessResource_->Swap(commandList);
	postProcessResource_->SetRenderTarget(commandList, BufferType::PONG, depthHandle_.handleCPU);
	
	CopyData();
	setting_->uv = param_.uvTransform.MakeAffine();

	Pipeline* pso = Engine::SetPipeline(PSOType::ProcessedScene, "PostProcess_Swirl.json");
	UINT index = pso->GetRootSignatureIndex("gSceenTexture");
	commandList->SetGraphicsRootDescriptorTable(index, postProcessResource_->GetPingResource()->GetSRV().handleGPU);
	/*index = pso->GetRootSignatureIndex("gPatternTexture");
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, param_.patternTextureName, index);*/
	/*index = pso->GetRootSignatureIndex("gSetting");
	commandList->SetGraphicsRootConstantBufferView(index, settingBuffer_->GetResource()->GetGPUVirtualAddress());*/
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

void SwirlEffect::CheckBox() {
	ImGui::Checkbox("SwirlEffect##SwirlEffectSwirlEffect_checkBox", &isEnable_);
}

void SwirlEffect::Debug_Gui() {
	if (ImGui::CollapsingHeader("Distortion##Distortion_checkBox")) {
		ImGui::DragFloat3("scale", &param_.uvTransform.scale.x, 0.1f);
		ImGui::DragFloat3("rotate", &param_.uvTransform.rotate.x, 0.1f);
		ImGui::DragFloat3("translate", &param_.uvTransform.translate.x, 0.1f);

		/*if (ImGui::Button("Save")) {
			param_.isEnable = isEnable_;
			JsonItems::Save("PostEffect", param_.ToJson(param_.GetName()));
		}
		if (ImGui::Button("Apply")) {
			param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
		}*/
	}
}

void SwirlEffect::ApplySaveData() {
}

void SwirlEffect::CopyData() {
}
