#include "SwirlMask.h"
#include "Engine.h"
#include "Engine/Core/GraphicsContext.h"

SwirlMask::~SwirlMask() {
}

void SwirlMask::Init() {
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init("white.png");
}

void SwirlMask::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
	Pipeline* pso = Engine::SetPipeline(PSOType::ProcessedScene, "PostProcess_Normal_16.json");
	UINT index = pso->GetRootSignatureIndex("gTexture");
	commandList->SetGraphicsRootDescriptorTable(index, pingResource->GetSRV().handleGPU);
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);

	//pso = Engine::SetPipeline(PSOType::Sprite, "Sprite_Normal_16.json");
	//sprite_->Draw(pso);
}

void SwirlMask::CheckBox() {
}

void SwirlMask::Debug_Gui() {
}
