#include "TitleUIs.h"
#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void TitleUIs::Init() {
	SetName("TitleUIs");
	title_ = Engine::GetCanvas2d()->AddSprite("title.png","Sprite_Normal.json");

	AddChild(title_);
	EditorWindows::AddObjectWindow(this, GetName());
}

void TitleUIs::Update() {

}

void TitleUIs::Debug_Gui() {

}
