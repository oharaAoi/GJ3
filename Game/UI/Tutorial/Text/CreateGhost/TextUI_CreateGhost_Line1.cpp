#include "TextUI_CreateGhost_Line1.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void TextUI_CreateGhost_Line1::Init()
{
	textUI_ = Engine::GetCanvas2d()->AddSprite("Tutorial_CreateGhostText01.png", GetName(), "Sprite_Normal.json", 200);

	SetName("TextUI_CreateGhost_Line1");
	AddChild(textUI_);
	EditorWindows::AddObjectWindow(textUI_, GetName());

	textUI_->ApplySaveData();
	textUI_->SetUvMaxSize(Vector2{ 1.0f,0.0f });

	isFinished_ = false;
}

void TextUI_CreateGhost_Line1::Update()
{
}

void TextUI_CreateGhost_Line1::Debug_Gui()
{
	textUI_->Debug_Gui();
}
