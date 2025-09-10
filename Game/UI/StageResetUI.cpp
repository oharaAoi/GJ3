#include "StageResetUI.h"

#include "Engine.h"
#include "Engine/System/Input/Input.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Engine/System/Audio/AudioPlayer.h"
#include "Engine/Lib/GameTimer.h"

void StageResetUI::Init(Canvas2d* _canvas2d)
{
	SetName("StageResetButtonUI");
	EditorWindows::AddObjectWindow(this, GetName());

	pCanvas2d_ = _canvas2d;
	resetButtonUI_ = pCanvas2d_->AddSprite("resetButton_R.png", "resetButton_R", "Sprite_Normal.json", 6);
	resetButtonUI_->ApplySaveData();
	AddChild(resetButtonUI_);

	resetIcon_ = pCanvas2d_->AddSprite("reset_text.png","ResetIcon","Sprite_Normal.json",7);
	resetIcon_->ApplySaveData();
	AddChild(resetIcon_);

	backTextureUI_ = pCanvas2d_->AddSprite("white.png", "backTexture", "Sprite_Normal.json", 5);
	backTextureUI_->ApplySaveData();
	backTextureUI_->ReSetTextureSize(backTextureUI_->GetTextureSize());
	//backTextureUI_->ApplySaveData();
	AddChild(backTextureUI_);

	param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));

	Update();
}

void StageResetUI::Update()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	keyInput_ = input->IsPressKey(DIK_R);
	padInput_ = input->IsPressButton(XInputButtons::BUTTON_X);
	// リセットボタンが長押しされているなら
	if (keyInput_ || padInput_) {
		if (!isPush_) { 
			isPush_ = true;
			scaleTimer_ = 0.0f;
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
		}
		resetTimer_ += (GameTimer::DeltaTime() / param_.resetTime);
		if (resetTimer_ > 1.0f) { isStageReset_ = true; }
	} else {
		resetTimer_ -= GameTimer::DeltaTime();
		isStageReset_ = false;
	}
	if (keyInput_) {
		resetButtonUI_->SetTextureName("resetButton_R.png");
	} else if (padInput_) {
		resetButtonUI_->SetTextureName("resetButton_X.png");
	}
	resetTimer_ = std::clamp(resetTimer_, 0.0f, 1.0f);
	backTextureUI_->SetUvMinSize({ 0.0f,1.0f - resetTimer_ });

	// 押したらぷにっと
	if (isPush_) {
		scaleTimer_ += GameTimer::DeltaTime() * 5.0f;
		scaleTimer_ = std::clamp(scaleTimer_, 0.0f, 1.0f);
		backTextureUI_->SetScale(Vector2::MochiPuniScaleNormalized(scaleTimer_));
		resetButtonUI_->SetScale(Vector2::MochiPuniScaleNormalized(scaleTimer_));
		if (scaleTimer_ == 1.0f && !keyInput_ && !padInput_) { isPush_ = false; }
	}
}

void StageResetUI::Debug_Gui()
{
	ImGui::DragFloat2("resetButtonPos", &param_.resetButtonPos.x, 0.1f);
	ImGui::DragFloat2("backTexturePos", &param_.backTexturePos.x, 0.1f);
	ImGui::ColorEdit4("backColor", &param_.backColor.x);
	ImGui::DragFloat("def_size", &param_.def_size, 0.1f);
	ImGui::DragFloat("resetTime", &param_.resetTime, 0.1f);

	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));
	}
}
