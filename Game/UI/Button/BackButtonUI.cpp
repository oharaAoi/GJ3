#include "BackButtonUI.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

void BackButtonUI::Init()
{
	SetName("BackButtonUI");
	buttonUI_ = Engine::GetCanvas2d()->AddSprite("Menu_backUI.png", GetName(), "Sprite_Normal.json",1011);

	//buttonUI_->ReSetTextureSize(Vector2{ 100.0f,50.0f });
	buttonUI_->SetTranslate(Vector2{ 640.0f,360.0f });
	type_ = ButtonType::Back;
}

void BackButtonUI::Update()
{
}

void BackButtonUI::Debug_Gui()
{
}
