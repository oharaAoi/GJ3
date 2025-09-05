#include "BackButtonUI.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void BackButtonUI::Init()
{
	SetName("BackButtonUI");
	buttonUI_ = Engine::GetCanvas2d()->AddSprite("white.png", "Sprite_Normal.json", 11);

	AddChild(buttonUI_);
	EditorWindows::AddObjectWindow(this, GetName());

	buttonUI_->ReSetTextureSize(Vector2{ 100.0f,50.0f });
	buttonUI_->SetTranslate(Vector2{ 640.0f,500.0f });
	type_ = MenuButtonType::Back;
}

void BackButtonUI::Update()
{
}

void BackButtonUI::Debug_Gui()
{
}
