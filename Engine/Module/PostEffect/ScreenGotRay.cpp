#include "ScreenGotRay.h"
#include "Engine.h"
#include "Engine/Core/GraphicsContext.h"
#include "Engine/System/Manager/TextureManager.h"

ScreenGotRay::~ScreenGotRay() {
	cBuffer_->Finalize();
}

void ScreenGotRay::Init() {
	GraphicsContext* graphicsCtx = GraphicsContext::GetInstance();
	cBuffer_ = std::make_unique<DxResource>();
	cBuffer_->Init(graphicsCtx->GetDevice(), graphicsCtx->GetDxHeap(), ResourceType::COMMON);
	cBuffer_->CreateResource(sizeof(GotRaySetting));
	cBuffer_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&setting_));

	setting_->color = { 1,1,1,1 };
	setting_->pos = { 0,0 };
	setting_->angle = 0;
	setting_->spread = 2.f;
	setting_->cutoff = 0.7f;
	setting_->falloff = 0.9f;
	setting_->edgeFade = 1.0f;
	setting_->speed = 5.0f;
	setting_->ray1Density = 1.0f;
	setting_->ray2Density = 0.0f;
	setting_->ray3Density = 1.0f;
	setting_->seed = 0.0f;
	setting_->time = 0.0f;

}

void ScreenGotRay::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
	CopyData();
	setting_->time = GameTimer::DeltaTime();

	Pipeline* pso = Engine::SetPipeline(PSOType::ProcessedScene, "PostProcess_GotRay.json");
	UINT index = pso->GetRootSignatureIndex("g_SceneTex");
	commandList->SetGraphicsRootDescriptorTable(index, pingResource->GetSRV().handleGPU);
	index = pso->GetRootSignatureIndex("gGotRay");
	commandList->SetGraphicsRootConstantBufferView(index, cBuffer_->GetResource()->GetGPUVirtualAddress());
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

void ScreenGotRay::CheckBox() {
	ImGui::Checkbox("GotRay##GotRay_checkBox", &isEnable_);
}

void ScreenGotRay::Debug_Gui() {
	if (ImGui::CollapsingHeader("GotRay##GotRay")) {
		// 色
		ImGui::ColorEdit4("Color", (float*)&param_.color);

		// 位置
		ImGui::DragFloat2("Position", (float*)&param_.pos, 0.01f);

		// 角度
		ImGui::SliderAngle("Angle", &param_.angle, -180.0f, 180.0f);

		// 広がり
		ImGui::DragFloat("Spread", &param_.spread, 0.01f, 0.0f, 5.0f);

		// 各種パラメータ
		ImGui::DragFloat("Cutoff", &param_.cutoff, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Falloff", &param_.falloff, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("EdgeFade", &param_.edgeFade, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Speed", &param_.speed, 0.01f, 0.0f, 5.0f);

		// レイ密度
		ImGui::DragFloat("Ray1 Density", &param_.ray1Density, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Ray2 Density", &param_.ray2Density, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Ray3 Density", &param_.ray3Density, 0.01f, 0.0f, 1.0f);

		// シード値
		ImGui::DragFloat("Seed", &param_.seed, 0.01f, 0.0f, 10.0f);

		// 時間（デバッグ用）
		ImGui::DragFloat("Time", &param_.time, 0.01f, 0.0f, 100.0f);

		if (ImGui::Button("Save##gotRay_save")) {
			param_.isEnable = isEnable_;
			JsonItems::Save("PostEffect", param_.ToJson(param_.GetName()));
		}
		if (ImGui::Button("Apply##gotRay_apply")) {
			param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
		}
	}
}

void ScreenGotRay::ApplySaveData() {
	param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
	isEnable_ = param_.isEnable;
	CopyData();
}

void ScreenGotRay::CopyData() {
	setting_->color = param_.color;
	setting_->pos = param_.pos;
	setting_->angle = param_.angle;
	setting_->spread = param_.spread;
	setting_->cutoff = param_.cutoff;
	setting_->falloff = param_.falloff;
	setting_->edgeFade = param_.edgeFade;
	setting_->speed = param_.speed;
	setting_->ray1Density = param_.ray1Density;
	setting_->ray2Density = param_.ray2Density;
	setting_->ray3Density = param_.ray3Density;
	setting_->seed = param_.seed;
}

void ScreenGotRay::SetRayDensity(float _value, uint16_t index) {
	if (index == 1) {
		param_.ray1Density = _value;
	} else if (index == 2) {
		param_.ray2Density = _value;
	} else {
		param_.ray3Density = _value;
	}
}

float ScreenGotRay::GetRayDensity(uint16_t index) {
	if (index == 1) {
		return param_.ray1Density;
	} else if (index == 2) {
		return param_.ray2Density;
	} else {
		return param_.ray3Density;
	}
}
