#include "StageContentCamera.h"
#include "Engine/Render.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

StageContentCamera::StageContentCamera() {
	stageSize_ = { kWindowWidth_ * 0.3f, kWindowHeight_ * 0.3f };
}

StageContentCamera::~StageContentCamera() {
}

void StageContentCamera::Init() {
	projectionMatrix_ = Matrix4x4::MakeOrthograhic(0.0f, 0.0f, stageSize_.x, stageSize_.y, near_, far_);
	viewMatrix_ = Matrix4x4::MakeUnit();
	viewportMatrix_ = Matrix4x4::MakeViewport(0, 0, kWindowWidth_, kWindowHeight_, 0, 1);

	EditorWindows::AddObjectWindow(this, "camera2d");
}

void StageContentCamera::Update() {
	// 4x4で表現するためz成分を削除
	translate_.z = 0;
	viewMatrix_ = Inverse(translate_.MakeTranslateMat());
	worldMat_ = Multiply(Multiply(Matrix4x4::MakeUnit(), Matrix4x4::MakeUnit()), translate_.MakeTranslateMat());
	// Renderに設定
	Render::SetViewProjection2D(viewMatrix_, projectionMatrix_);
}

void StageContentCamera::Debug_Gui() {
	ImGui::DragFloat("near", &near_, 0.1f);
	ImGui::DragFloat("far", &far_, 0.1f);
	ImGui::DragFloat2("translate", &translate_.x, 1.0f);

	projectionMatrix_ = Matrix4x4::MakeOrthograhic(0.0f, 0.0f, stageSize_.x, stageSize_.y, near_, far_);
}
