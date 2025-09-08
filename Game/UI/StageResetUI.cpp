#include "StageResetUI.h"

#include "Engine.h"
#include "Engine/System/Input/Input.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Engine/Lib/GameTimer.h"

void StageResetUI::Init(Canvas2d* _canvas2d)
{
	pCanvas2d_ = _canvas2d;
	resetButtonUI_ = pCanvas2d_->AddSprite("resetButton_R.png", GetName(), "Sprite_Normal.json", 6);
	//resetButtonUI_->SetAnchorPoint(Vector2{ 1.0f,1.0f });

	backTextureUI_ = pCanvas2d_->AddSprite("white.png", GetName(), "Sprite_Normal.json", 5);
	//backTextureUI_->SetAnchorPoint(Vector2{ 0.5f,1.0f });
	backTextureUI_->SetColor(Vector4{ 1.0f,0.0f,0.0f,1.0f });

	whiteTextureUI_ = pCanvas2d_->AddSprite("white.png", GetName(), "Sprite_Normal.json", 4);
	//whiteTextureUI_->SetAnchorPoint(Vector2{ 1.0f,1.0f });

	SetName("StageResetButtonUI");
	AddChild(this);
	EditorWindows::AddObjectWindow(this, GetName());
	param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));

	backTextureUI_->ReSetTextureSize(textureSize_);
	whiteTextureUI_->ReSetTextureSize(textureSize_);
	resetButtonUI_->ReSetTextureSize(textureSize_);

	Vector2 windowSize = { kWindowWidth_, kWindowHeight_ };
	backTextureUI_->SetTranslate(Vector2{ windowSize - textureSize_ });
	resetButtonUI_->SetTranslate(Vector2{ windowSize - textureSize_ });
	whiteTextureUI_->SetTranslate(Vector2{ windowSize - textureSize_ });

	Update();
}

void StageResetUI::Update()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	bool keyInput = input->IsPressKey(DIK_R);
	bool padInput = input->IsPressButton(XInputButtons::BUTTON_X);
	// リセットボタンが長押しされているなら
	if (keyInput || padInput) {
		resetTimer_ += (GameTimer::DeltaTime() / param_.resetTime);
		if (resetTimer_ > 1.0f) { isStageReset_ = true; }
	} else {
		resetTimer_ -= GameTimer::DeltaTime();
		isStageReset_ = false;
	}
	if (keyInput) {
		resetButtonUI_->SetTextureName("resetButton_R.png");
	} else if (padInput) {
		resetButtonUI_->SetTextureName("resetButton_X.png");
	}
	resetTimer_ = std::clamp(resetTimer_, 0.0f, 1.0f);
	backTextureUI_->SetUvMinSize({ 0.0f,1.0f - resetTimer_ });

}

void StageResetUI::Debug_Gui()
{
	ImGui::DragFloat2("resetButtonPos", &param_.resetButtonPos.x, 0.1f);
	ImGui::DragFloat2("backTexturePos", &param_.backTexturePos.x, 0.1f);
	ImGui::DragFloat("def_size", &param_.def_size, 0.1f);
	ImGui::DragFloat("resetTime", &param_.resetTime, 0.1f);

	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));
	}
}
