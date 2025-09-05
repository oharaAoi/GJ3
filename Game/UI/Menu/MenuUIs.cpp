#include "MenuUIs.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Engine/Lib/GameTimer.h"

#include "Game/UI/Menu/button/SelectButtonUI.h"
#include "Game/UI/Menu/button/ResetButtonUI.h"
#include "Game/UI/Menu/button/OperationButtonUI.h"
#include "Game/UI/Menu/button/BackButtonUI.h"

void MenuUIs::Init()
{
	SetName("MenuUIs");
	menu_ = Engine::GetCanvas2d()->AddSprite("menu.png", GetName(), "Sprite_Normal.json", 10);
	AddChild(menu_);
	EditorWindows::AddObjectWindow(this, GetName());
	menu_->SetTranslate(Vector2{ 640.0f,360.0f });

	operationUI_ = std::make_unique<OperationUI>();
	operationUI_->Init();

	// ボタン生成、初期化
	buttonUIs_[0] = std::make_unique<SelectButtonUI>();
	buttonUIs_[1] = std::make_unique<ResetButtonUI>();
	buttonUIs_[2] = std::make_unique<OperationButtonUI>();
	buttonUIs_[3] = std::make_unique<BackButtonUI>();
	// 初期化
	for (auto& buttonUI : buttonUIs_) {
		buttonUI->Init();
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
	for (auto& buttonUI : buttonUIs_) {
		buttonUI->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,alpha });
	}
}

void MenuUIs::Debug_Gui()
{
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
