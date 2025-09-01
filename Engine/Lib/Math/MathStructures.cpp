#include "MathStructures.h"

void OBB::MakeOBBAxis(const Quaternion& rotate) {
	Matrix4x4 rotateMatrix = rotate.MakeMatrix();

	// 回転行列から軸を抽出
	orientations[0].x = rotateMatrix.m[0][0];
	orientations[0].y = rotateMatrix.m[0][1];
	orientations[0].z = rotateMatrix.m[0][2];

	orientations[1].x = rotateMatrix.m[1][0];
	orientations[1].y = rotateMatrix.m[1][1];
	orientations[1].z = rotateMatrix.m[1][2];

	orientations[2].x = rotateMatrix.m[2][0];
	orientations[2].y = rotateMatrix.m[2][1];
	orientations[2].z = rotateMatrix.m[2][2];

	matRotate = rotateMatrix;
}

std::vector<Vector3> OBB::MakeIndex() const {
	std::vector<Vector3> vertices;
	for (uint8_t x = 0; x < 2; ++x) {
		for (uint8_t y = 0; y < 2; ++y) {
			for (uint8_t z = 0; z < 2; ++z) {
				Vector3 localVertex = {
					(x ? size.x : -size.x),
					(y ? size.y : -size.y),
					(z ? size.z : -size.z),
				};

				Vector3 worldVertex = Transform(localVertex, matRotate);
				vertices.push_back(worldVertex + center);
			}
		}
	}
	return vertices;
}

Matrix4x4 SRT::MakeAffine() {
	Matrix4x4 scaleMatrix{};
	Matrix4x4 rotateMatrix{};
	Matrix4x4 translateMatrix{};
	Matrix4x4 affineMatrix{};

	scaleMatrix = scale.MakeScaleMat();
	rotateMatrix = rotate.MakeRotateMat();
	translateMatrix = translate.MakeTranslateMat();

	affineMatrix = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return affineMatrix;
}

Matrix4x4 QuaternionSRT::MakeAffine() {
	Matrix4x4 scaleMatrix{};
	Matrix4x4 rotateMatrix{};
	Matrix4x4 translateMatrix{};
	
	scaleMatrix = scale.MakeScaleMat();
	rotateMatrix = rotate.MakeMatrix();
	translateMatrix = translate.MakeTranslateMat();

	worldMat_ = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	if (parentWorldMat != nullptr) {
		worldMat_ = worldMat_ * *parentWorldMat;
	}

	return worldMat_;
}

void QuaternionSRT::SetParent(const Matrix4x4& parentMat) {
	parentWorldMat = &parentMat;
}

void QuaternionSRT::LookAt(const Vector3& target, const Vector3& up) {
	Vector3 direction = target - translate;

	if (direction.Length() > 0.0001f) {
		rotate = Quaternion::LookRotation(direction, up);
	}
}