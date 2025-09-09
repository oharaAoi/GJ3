#include "Vector2.h"

float Vector2::Length() const {
	return std::sqrt((x * x) + (y * y));
}

float Vector2::LengthSq() const{
	return (x * x) + (y * y);
}


Vector2 Vector2::Normalize() const {
	Vector2 result = { x, y };
	float len = this->Length();
	if (len != 0) {
		result.x /= len;
		result.y /= len;
	}
	return result;
}

void Vector2::Clamp(const Vector2& min, const Vector2& max) {
	if (min.x > max.x) {
		x = std::clamp(x, min.x, max.x);
	}

	if (min.y > max.y) {
		y = std::clamp(y, min.y, max.y);
	}
}

float Vector2::Dot(const Vector2& v1, const Vector2& v2) {
	return ((v1.x * v2.x) + (v1.y * v2.y));
}

float Vector2::Cross(const Vector2& v1, const Vector2& v2) {
	return (v1.x * v2.y) - (v2.x * v1.y);
}

Vector2 Vector2::Lerp(const Vector2& st, const Vector2& end, float t) {
	return {
		std::lerp(st.x, end.x, t),
		std::lerp(st.y, end.y, t)
	};
}

Matrix3x3 Vector2::MakeScaleMat(const Vector2& scale) {
	Matrix3x3 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = 1;

	return result;
}

Matrix3x3 Vector2::MakeRotateMat(float theta) {
	Matrix3x3 result{};
	result.m[0][0] = cosf(theta);
	result.m[0][1] = sinf(theta);
	result.m[1][0] = -sinf(theta);
	result.m[1][1] = cosf(theta);
	result.m[2][2] = 1;

	return result;
}

Matrix3x3 Vector2::MakeTranslateMat(const Vector2& translate) {
	Matrix3x3 result{};
	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][0] = translate.x;
	result.m[2][1] = translate.y;
	result.m[2][2] = 1;

	return result;
}

Matrix3x3 Vector2::MakeAffine(const Vector2& scale, float theta, const Vector2& translate) {
	Matrix3x3 scaleMatrix{};
	Matrix3x3 rotateMatrix{};
	Matrix3x3 translateMatrix{};
	Matrix3x3 worldMatrix{};

	scaleMatrix = MakeScaleMat(scale);
	rotateMatrix = MakeRotateMat(theta);
	translateMatrix = MakeTranslateMat(translate);

	worldMatrix = Matrix3x3::Multiply(Matrix3x3::Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return worldMatrix;
}

Vector2 Vector2::MochiPuniScaleNormalized(float time, float amplitude, float damping)
{
	time = std::clamp(time, 0.0f, 1.0f);

	const float pi = 3.14159265358979323846f;
	const float twoPi = 6.2831853071795864769f;

	// 0と1で必ず0になる包絡
	float envelope = std::sin(pi * time);

	// 減衰（0ならそのまま）
	float decay = (damping > 0.0f) ? std::exp(-damping * time) : 1.0f;

	// X: 通常のsin波, Y: 位相を進める（ただし必ずenvelopeで0終端）
	float sx = 1.0f + amplitude * std::sin(twoPi * time) * envelope * decay;
	float sy = 1.0f + amplitude * std::sin(twoPi * time + pi * 0.5f) * envelope * decay;

	return { sx, sy };
}
