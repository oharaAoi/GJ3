#include "StageContents.h"
#include "Engine/Core/GraphicsContext.h"

StageContents::~StageContents() {
}

void StageContents::Init() {
	stageRenderTarget_ = std::make_unique<StageRenderTarget>();
	stageRenderTarget_->Init();

	camera_ = std::make_unique<StageContentCamera>();
	camera_->Init();

	for (uint32_t i = 0; i < 3; ++i) {
		canvas2ds_[i] = std::make_unique<Canvas2d>();
		canvas2ds_[i]->Init();

		player_[i] = std::make_unique<Player>();
		player_[i]->Init(canvas2ds_[i].get());

		stageRegistries_[i] = std::make_unique<StageRegistry>();
		stageRegistries_[i]->Init(canvas2ds_[i].get());
		stageRegistries_[i]->SetPlayer(player_[i].get());
		stageRegistries_[i]->SetWindowSize({ kWindowWidth_ * 0.3f, kWindowHeight_ * 0.3f });
		stageRegistries_[i]->SetTileRatio(0.9f);
		stageRegistries_[i]->Register("stage_0.json");
	}
}

void StageContents::Update() {
	camera_->Update();

	for (uint32_t i = 0; i < 3; ++i) {
		stageRenderTarget_->Transition(GraphicsContext::GetInstance()->GetCommandList(), i, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

		stageRegistries_[i]->Update();
		canvas2ds_[i]->Update();

		stageRenderTarget_->SetRenderTarget(GraphicsContext::GetInstance()->GetCommandList(), i);
		canvas2ds_[i]->Draw();
		stageRenderTarget_->Transition(GraphicsContext::GetInstance()->GetCommandList(), i, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	}
}