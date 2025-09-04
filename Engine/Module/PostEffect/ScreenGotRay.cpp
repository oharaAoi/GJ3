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
	cBuffer_->CreateResource(sizeof(GotRayParameter));
	cBuffer_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&setting_));

	setting_->color = { 1,1,1,1 };
	setting_->pos = { 0,0 };
	setting_->angle = kPI;
	setting_->spread = 1.f;
	setting_->cutoff = 0.5f;
	setting_->falloff = 0.3f;
	setting_->edgeFade = 0.3f;
	setting_->speed = 1.0f;
	setting_->ray1Density = 0.0f;
	setting_->ray2Density = 0.0f;
	setting_->ray3Density = 0.0f;
	setting_->seed = 0.0f;
	setting_->time = 0.0f;

}

void ScreenGotRay::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
}

void ScreenGotRay::CheckBox() {
	ImGui::Checkbox("GotRay##GotRay_checkBox", &isEnable_);
}

void ScreenGotRay::Debug_Gui() {
	if (ImGui::CollapsingHeader("GotRay##GotRay")) {
		
	}
}
