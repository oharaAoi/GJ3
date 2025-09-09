#include "WorldObjects.h"
#include "Engine.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void WorldObjects::Init() {
	SetName("WorldObjects");
	backGround_ = std::make_unique<BackGround>();
	backGround_->Init(Engine::GetCanvas2d());

	AddChild(backGround_.get());

	EditorWindows::AddObjectWindow(this, GetName());
}

void WorldObjects::Update() {

}

void WorldObjects::Debug_Gui() {
}
