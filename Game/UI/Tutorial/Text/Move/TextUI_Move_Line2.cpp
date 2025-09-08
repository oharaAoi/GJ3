#include "TextUI_Move_Line2.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void TextUI_Move_Line2::Init()
{
	textUI_ = Engine::GetCanvas2d()->AddSprite("Tutorial_MoveText02.png", GetName(), "Sprite_Normal.json", 200);

	SetName("TextUI_Move_Line2");
	AddChild(textUI_);
	EditorWindows::AddObjectWindow(textUI_, GetName());

	textUI_->ApplySaveData();
	textUI_->SetUvMaxSize(Vector2{ 1.0f,0.0f });

	isFinished_ = false;
}

void TextUI_Move_Line2::Update()
{
}

void TextUI_Move_Line2::Debug_Gui()
{
	textUI_->Debug_Gui();
}
