#include "WorldObjects.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void WorldObjects::Init() {
	SetName("WorldObjects");
	backGround_ = std::make_unique<BackGround>();

	EditorWindows::AddObjectWindow(this, GetName());
}

void WorldObjects::Update() {

}

void WorldObjects::Debug_Gui() {
}
