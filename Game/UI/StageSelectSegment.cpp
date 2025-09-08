#include "StageSelectSegment.h"
#include "Engine.h"

void StageSelectSegment::Init(Canvas2d* _canvas2d, uint32_t index) {
	std::string thisName = "StageSelectSegment_" + std::to_string(index);
	SetName(thisName.c_str());
	GraphicsContext* ctx = GraphicsContext::GetInstance();

	RenderTargetType type;
	if (index == 1) {
		type = RenderTargetType::Stage_RenderTarget1;
	} else if (index == 2) {
		type = RenderTargetType::Stage_RenderTarget2;
	} else {
		type = RenderTargetType::Stage_RenderTarget3;
	}

	backGround_ = _canvas2d->AddSprite("Select_bg.png", "Background", "Sprite_Normal.json", -1, true);
	pictureFrame_ = _canvas2d->AddSprite("Select_stage_frame.png", "Frame", "Sprite_Normal.json", 1, true);
	stageContent_ = Engine::GetCanvas2d()->AddSprite("white.png", "StagePreview", "Sprite_Normal.json", 2, true);
	stageContent_->SetTextureResource(ctx->GetRenderTarget()->GetRenderTargetResource(type));
	stageContent_->ReSetTextureSize({ kWindowWidth_ * 0.3f,kWindowHeight_ * 0.3f });

	// 座標を設定
	backGround_->SetTranslate(centerPos_);
	pictureFrame_->SetTranslate(centerPos_);
	stageContent_->SetTranslate(centerPos_);
}

void StageSelectSegment::Update() {

}

void StageSelectSegment::Debug_Gui() {
	backGround_->Debug_Gui();
	pictureFrame_->Debug_Gui();
	stageContent_->Debug_Gui();
}
