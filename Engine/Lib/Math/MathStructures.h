#pragma once
#include <vector>
#include "MyMath.h"
#include "Engine/Lib/Math/Quaternion.h"

/// <summary>
/// 拡縮、回転、移動
/// </summary>
struct SRT {
	Vector3 scale = CVector3::UNIT;
	Vector3 rotate = CVector3::ZERO;
	Vector3 translate = CVector3::ZERO;

	Matrix4x4 MakeAffine();
};

struct QuaternionSRT {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;

	Matrix4x4 worldMat_;
	const Matrix4x4* parentWorldMat = nullptr;

	// 関数
	Matrix4x4 MakeAffine();

	void SetParent(const Matrix4x4& parentMat);

	void LookAt(const Vector3& target, const Vector3& up = CVector3::UP);

};

/// <summary>
/// 球
/// </summary>
struct Sphere {
	Vector3 center;
	float radius;
};

/// <summary>
/// 軸平行境界箱
/// </summary>
struct AABB {
	Vector3 min;
	Vector3 max;
	Vector3 center; // 中心点
};

/// <summary>
/// OBB(有向境界箱)
/// </summary>
struct OBB {
	Vector3 center; // 中心点
	Vector3 orientations[3]; // 座標軸、正規化、直交必須
	Vector3 size; // 座標軸方向の長さの半分

	// 回転軸
	Matrix4x4 matRotate;

	void MakeOBBAxis(const Quaternion& rotate);
	std::vector<Vector3> MakeIndex() const;
};
