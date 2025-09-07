#pragma once
#include "Game/Camera/BaseCamera.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

/// <summary>
/// stage描画用のカメラ
/// </summary>
class StageContentCamera :
	public BaseCamera,
	public AttributeGui {
public:

	StageContentCamera();
	~StageContentCamera() override;

	void Finalize() override {};
	void Init() override;
	void Update() override;

	void Debug_Gui() override;

public:

	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }

	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	Matrix4x4 GetVPVMatrix() const { return worldMat_ * projectionMatrix_ * viewportMatrix_; }

private:

	float near_ = 0.0f;
	float far_ = 100.0f;

	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 viewportMatrix_;
	Matrix4x4 worldMat_;

	Vector3 translate_;

	Vector2 stageSize_;
};

