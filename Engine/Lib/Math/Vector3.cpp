#include "Vector3.h"


Vector3 Vector3::Normalize() const {
    float length = Length();
    if (length == 0.0f) {
        return *this;
    }
    return Vector3(x / length, y / length, z / length);
}

float Vector3::Length() const {
    return std::sqrtf(x * x + y * y + z * z);
}

void Vector3::Clamp(const Vector3& min, const Vector3& max) {
    if (min.x >= max.x) {
        x = max.x;
    }

    if (min.y >= max.y) {
        y = max.y;
    }

    if (min.z >= max.z) {
        z = max.z;
    }

    x = std::clamp(x, min.x, max.x);
    y = std::clamp(y, min.y, max.y);
    z = std::clamp(z, min.z, max.z);
}

Matrix4x4 Vector3::MakeScaleMat() const {
    Matrix4x4 result{};
    result.m[0][0] = x;
    result.m[1][1] = y;
    result.m[2][2] = z;
    result.m[3][3] = 1;
    return result;
}

Matrix4x4 Vector3::MakeRotateMat() const {
    Matrix4x4 result{};
    result = MakePitchMatrix(x) * (MakeYawMatrix(y) * MakeRollMatrix(z));
    return result;
}

Matrix4x4 Vector3::MakeTranslateMat() const {
    Matrix4x4 result{};
    result.m[0][0] = 1;
    result.m[1][1] = 1;
    result.m[2][2] = 1;
    result.m[3][0] = x;
    result.m[3][1] = y;
    result.m[3][2] = z;
    result.m[3][3] = 1;
    return result;
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
    return std::clamp((v2.x * v1.x) + (v2.y * v1.y) + (v2.z * v1.z), -1.0f, 1.0f);
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
    Vector3 result{};
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float t) {
    Vector3 result{};
    result.x = std::lerp(start.x, end.x, t);
    result.y = std::lerp(start.y, end.y, t);
    result.z = std::lerp(start.z, end.z, t);
    return result;
}

Vector3 Vector3::Min(const Vector3& min, const Vector3& target) {
    Vector3 result{};
    result.x = std::min(min.x, target.x);
    result.y = std::min(min.y, target.y);
    result.z = std::min(min.z, target.z);
    return result;
}

Vector3 Vector3::Max(const Vector3& max, const Vector3& target) {
    Vector3 result{};
    result.x = std::max(max.x, target.x);
    result.y = std::max(max.y, target.y);
    result.z = std::max(max.z, target.z);
    return result;
}

Matrix4x4 Vector3::MakeRollMatrix(float z_Roll) const {
    Matrix4x4 result{};
    result.m[0][0] = std::cos(z_Roll);
    result.m[0][1] = std::sin(z_Roll);
    result.m[1][0] = -std::sin(z_Roll);
    result.m[1][1] = std::cos(z_Roll);
    result.m[2][2] = 1;
    result.m[3][3] = 1;

    return result;
}

Matrix4x4 Vector3::MakePitchMatrix(float x_Picth) const {
    Matrix4x4 result{};
    result.m[0][0] = 1;
    result.m[1][1] = std::cos(x_Picth);
    result.m[1][2] = std::sin(x_Picth);
    result.m[2][1] = -std::sin(x_Picth);
    result.m[2][2] = std::cos(x_Picth);
    result.m[3][3] = 1;

    return result;
}

Matrix4x4 Vector3::MakeYawMatrix(float y_Yaw) const {
    Matrix4x4 result{};
    result.m[0][0] = std::cos(y_Yaw);
    result.m[0][2] = -std::sin(y_Yaw);
    result.m[1][1] = 1;
    result.m[2][0] = std::sin(y_Yaw);
    result.m[2][2] = std::cos(y_Yaw);
    result.m[3][3] = 1;

    return result;
}
