#include "StageContents.h"
#include "Engine/Core/GraphicsContext.h"

StageContents::~StageContents() {
}

void StageContents::Init(uint32_t maxStageNum) {
	maxStageNum_ = maxStageNum;

	/*stageRenderTarget_ = std::make_unique<StageRenderTarget>();
	stageRenderTarget_->Init();*/

	camera_ = std::make_unique<StageContentCamera>();
	camera_->Init();

	canvas2ds_.resize(maxStageNum_);
	stageRegistries_.resize(maxStageNum_);
	for (uint32_t i = 0; i < maxStageNum_; ++i) {
		canvas2ds_[i] = std::make_unique<Canvas2d>();
		canvas2ds_[i]->Init();

		stageRegistries_[i] = std::make_unique<StageRegistry>();
		stageRegistries_[i]->Init(canvas2ds_[i].get());
		stageRegistries_[i]->SetWindowSize({ kWindowWidth_ * 0.3f, kWindowHeight_ * 0.3f });
		stageRegistries_[i]->SetTileRatio(0.9f);
		std::string stageName = "stage_" + std::to_string(i + 1) + ".json";
		stageRegistries_[i]->Register(stageName);
	}

	ResetCurrentDrawIndex(0);
}

void StageContents::ResetCurrentDrawIndex(int32_t currentStageIndex) {
	drawIndex_.clear();
	int leftIndex = (currentStageIndex - 1 + maxStageNum_) % maxStageNum_;
	int centerIndex = currentStageIndex;
	int rightIndex = (currentStageIndex + 1) % maxStageNum_;
	drawIndex_.push_back(leftIndex);
	drawIndex_.push_back(centerIndex);
	drawIndex_.push_back(rightIndex);
}

void StageContents::Update() {
	camera_->Update();
	GraphicsContext* ctx = GraphicsContext::GetInstance();
	pStageCollection_->SortSegments();

	for (uint32_t i = 0; i < drawIndex_.size(); ++i) {
		int index = drawIndex_[i];
		// RenderTargetを作成
		std::vector<RenderTargetType> postRenderTypes;
		// segmentの座標から順番を割り出す
		RenderTargetType type = pStageCollection_->GetRenderTarget(i);
		postRenderTypes.push_back(type);

		// RTVに変更
		ctx->GetRenderTarget()->TransitionResource(ctx->GetCommandList(), type, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		stageRegistries_[index]->Update();
		canvas2ds_[index]->Update();
		
		Render::SetRenderTarget(postRenderTypes, ctx->GetDxCommon()->GetDepthHandle());
		canvas2ds_[index]->Draw();
		ctx->GetRenderTarget()->TransitionResource(ctx->GetCommandList(), type, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	}
}