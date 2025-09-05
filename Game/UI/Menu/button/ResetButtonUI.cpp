#include "ResetButtonUI.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void ResetButtonUI::Init()
{
	SetName("ResetButtonUI");
	buttonUI_ = Engine::GetCanvas2d()->AddSprite("white.png", "Sprite_Normal.json", 11);

	AddChild(buttonUI_);
	EditorWindows::AddObjectWindow(this, GetName());

	buttonUI_->ReSetTextureSize(Vector2{ 400.0f,50.0f });
	buttonUI_->SetTranslate(Vector2{ 640.0f,300.0f });
	type_ = MenuButtonType::Reset;
}

void ResetButtonUI::Update()
{
}

void ResetButtonUI::Debug_Gui()
{
}
