#include "MenuUIs.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

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
}

void MenuUIs::Debug_Gui()
{
}

void MenuUIs::EnableDraw(bool enable)
{
	menu_->SetEnable(enable);
	for (auto& buttonUI : buttonUIs_) {
		buttonUI->GetSprite()->SetEnable(enable);
	}
}
