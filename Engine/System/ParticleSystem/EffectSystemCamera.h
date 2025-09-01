#pragma once
#include "Game/Camera/BaseCamera.h"
#include "Engine/Lib/Math/Quaternion.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

/// <summary>
/// Effectを作成する際に使用するカメラ
/// </summary>
class EffectSystemCamera :
	public BaseCamera,
	public AttributeGui {
public:

	EffectSystemCamera();
	~EffectSystemCamera();

	void Finalize() override;
	void Init() override;
	void Update() override;

	void Debug_Gui() override;

	/// <summary>
	/// カメラを動かす
	/// </summary>
	void TransitionMove();

	/// <summary>
	/// カメラを回転させる
	/// </summary>
	void RotateMove();

public:

	const Matrix4x4 GetCameraMatrix() const { return cameraMatrix_; }
	const Matrix4x4 GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	void SetIsFocused(bool isFocesd) { isFocused_ = isFocesd; }
	const bool GetIsFocused() const { return isFocused_; }

private:

	Quaternion quaternion_;
	// 回転する前のQuaternion
	Quaternion moveQuaternion_;

	bool isFocused_;

	// ---------------------------------------------------------------
	// ↓ デバックカメラで使う変数
	// ---------------------------------------------------------------
	bool debugCameraMode_ = true;

	float moveBaseSpeed_;
	float moveSpeed_;
	float moveMaxSpeed_ = 30.0f;
	Vector3 moveDirection_;
	Vector2 preMousePos_;
	
	float yaw_ = 0.0f;
	float pitch_ = 0.0f;
	float sensitivity_ = 0.05f; // 回転感度

	Quaternion qYaw;
	Quaternion qPitch;
};

