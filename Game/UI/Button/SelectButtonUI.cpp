#include "SelectButtonUI.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

void SelectButtonUI::Init()
{
	SetName("SelectButtonUI");
	buttonUI_ = Engine::GetCanvas2d()->AddSprite("white.png", GetName(), "Sprite_Normal.json", 11);

	buttonUI_->ReSetTextureSize(Vector2{ 400.0f,50.0f });
	buttonUI_->SetTranslate(Vector2{ 640.0f,200.0f });
	type_ = ButtonType::Select;
}

void SelectButtonUI::Update()
{
	// 点滅させる
	IButtonUI::Blinking();
}

void SelectButtonUI::Debug_Gui()
{
}
