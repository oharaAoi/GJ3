#include "NextStageButtonUI.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

void NextStageButtonUI::Init()
{
	SetName("NextStageButtonUI");
	buttonUI_ = Engine::GetCanvas2d()->AddSprite("white.png", GetName(), "Sprite_Normal.json", 11);

	buttonUI_->ReSetTextureSize(Vector2{ 400.0f,50.0f });
	buttonUI_->SetTranslate(Vector2{ 640.0f,200.0f });
	type_ = ButtonType::NextStage;
}

void NextStageButtonUI::Update()
{
}

void NextStageButtonUI::Debug_Gui()
{
}
