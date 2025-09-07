#include "ResetButtonUI.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

void ResetButtonUI::Init()
{
	SetName("ResetButtonUI");
	buttonUI_ = Engine::GetCanvas2d()->AddSprite("white.png", GetName(), "Sprite_Normal.json", 11);

	buttonUI_->ReSetTextureSize(Vector2{ 400.0f,50.0f });
	buttonUI_->SetTranslate(Vector2{ 640.0f,300.0f });
	type_ = ButtonType::Reset;
}

void ResetButtonUI::Update()
{
}

void ResetButtonUI::Debug_Gui()
{
}
