#include "MenuUIs.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Engine/Lib/GameTimer.h"

#include "Game/UI/Button/SelectButtonUI.h"
#include "Game/UI/Button/ResetButtonUI.h"
#include "Game/UI/Button/OperationButtonUI.h"
#include "Game/UI/Button/BackButtonUI.h"

void MenuUIs::Init()
{
	SetName("MenuUIs");
	AddChild(this);
	EditorWindows::AddObjectWindow(this, GetName());

	// ボタン生成、初期化
	menu_ = Engine::GetCanvas2d()->AddSprite("menu.png", GetName(), "Sprite_Normal.json", 10);
	menu_->SetTranslate(Vector2{ 640.0f,360.0f });
	buttonUIs_[0] = std::make_unique<SelectButtonUI>();
	buttonUIs_[1] = std::make_unique<ResetButtonUI>();
	buttonUIs_[2] = std::make_unique<OperationButtonUI>();
	buttonUIs_[3] = std::make_unique<BackButtonUI>();
	// 初期化
	for (auto& buttonUI : buttonUIs_) {
		buttonUI->Init();
	}

	operationUI_ = std::make_unique<OperationUI>();
	operationUI_->Init();

	param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));
	for (size_t i = 0; i < buttonUIs_.size(); ++i) {
		Vector2 position = {
			param_.centerPos.x,
			param_.centerPos.y + param_.interval * static_cast<float>(i)
		};
		if (i == 3) { position = param_.centerPos + param_.backButtonPos; }
		buttonUIs_[i]->GetSprite()->SetTranslate(position);
	}
}

void MenuUIs::Update()
{
	if (isFade_) {
		fadeFrame_ += GameTimer::DeltaTime();	// FadeIn
	} else {
		fadeFrame_ -= GameTimer::DeltaTime();	// FadeOut
	}
	// 範囲外に行かないように
	float alpha = fadeFrame_ = std::clamp(fadeFrame_, 0.0f, 1.0f);
	if (fadeFrame_ == 0.0f || fadeFrame_ == 1.0f) { endFade_ = true; }
	// alphaをセットする
	menu_->SetColor(Vector4{ 1.0f,1.0f,1.0f,alpha });
	for (size_t i = 0; i < buttonUIs_.size(); ++i) {
		Vector2 position = {
			param_.centerPos.x,
			param_.centerPos.y + param_.interval * static_cast<float>(i)
		};
		if (i == 3) { position = param_.centerPos + param_.backButtonPos; }
		buttonUIs_[i]->GetSprite()->SetTranslate(position);
		buttonUIs_[i]->GetSprite()->SetColor(Vector4{1.0f,1.0f,1.0f,alpha});
	}
}

void MenuUIs::Debug_Gui()
{
	ImGui::DragFloat2("centerPos", &param_.centerPos.x, 0.1f);
	ImGui::DragFloat("interval", &param_.interval, 0.1f);
	ImGui::DragFloat2("backButtonPos", &param_.backButtonPos.x, 0.1f);

	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));
	}
}

void MenuUIs::ResetUIs()
{
	isFade_ = false;
	endFade_ = true;
	fadeFrame_ = 0.0f;
	menu_->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.0f });
	for (auto& buttonUI : buttonUIs_) {
		buttonUI->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.0f });
	}
}
