#include "StageSelectCamera.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void StageSelectCamera::Init(Camera2d* _camera2d) {
	SetName("StageSelectCamera");
	pCamera2d_ = _camera2d;

	EditorWindows::AddObjectWindow(this, "StageSelectCamera");
}

void StageSelectCamera::Update() {
}

void StageSelectCamera::Debug_Gui() {
	pCamera2d_->Debug_Gui();
}
