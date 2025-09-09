#include "StageSelectSegment.h"
#include "Engine.h"

void StageSelectSegment::Init(Canvas2d* _canvas2d, int index) {
	std::string thisName = "StageSelectSegment_" + std::to_string(index);
	SetName(thisName.c_str());
	ctx_ = GraphicsContext::GetInstance();

	RenderTargetType type;
	if (index == 0) {
		type = RenderTargetType::Stage_RenderTarget1;
	} else if (index == 1) {
		type = RenderTargetType::Stage_RenderTarget2;
	} else {
		type = RenderTargetType::Stage_RenderTarget3;
	}
	rtType_ = type;

	backGround_ = _canvas2d->AddSprite("Select_bg.png", "Background", "Sprite_Normal.json", -1, true);
	pictureFrame_ = _canvas2d->AddSprite("Select_stage_frame.png", "Frame", "Sprite_Normal.json", 1, true);
	stageContent_ = _canvas2d->AddSprite("white.png", "StagePreview", "Sprite_Normal.json", 2, true);
	stageContent_->SetTextureResource(ctx_->GetRenderTarget()->GetRenderTargetResource(type));
	stageContent_->ApplySaveData();
	stageContent_->ReSetTextureSize({ kWindowWidth_ * 0.34f ,kWindowHeight_ * 0.34f});

	backGround_->SetColor({0.494f,0.494f,0.494f,1.f});
	pictureFrame_->SetColor({0.67f,0.67f,0.67f,1.f});

	// 座標を設定
	centerPos_.x = (kWindowWidth_ * 0.5f) + (kWindowWidth_ * (index - 1));
	backGround_->SetTranslate(centerPos_);
	pictureFrame_->SetTranslate(centerPos_);
	
	Vector2 contentCenter = stageContent_->GetTranslate();
	contentCenter.x = centerPos_.x;
	stageContent_->SetTranslate(contentCenter);
}

void StageSelectSegment::Update() {

}

void StageSelectSegment::Debug_Gui() {
	if (ImGui::TreeNode("backGround")) {
		backGround_->Debug_Gui();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("pictureFrame")) {
		pictureFrame_->Debug_Gui();
		ImGui::TreePop();
	}if (ImGui::TreeNode("stageContent")) {
		stageContent_->Debug_Gui();
		ImGui::TreePop();
	}
}

void StageSelectSegment::SetCenterPosX(float _posX) {
	centerPos_.x = _posX;
	backGround_->SetTranslate(centerPos_);
	pictureFrame_->SetTranslate(centerPos_);
	Vector2 contentCenter = stageContent_->GetTranslate();
	contentCenter.x = centerPos_.x;
	stageContent_->SetTranslate(contentCenter);
}
