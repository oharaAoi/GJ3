
static const float PI = 3.141592653589f;

float4x4 MakeUnit() {
	return float4x4(1.0, 0.0, 0.0, 0.0,
                    0.0, 1.0, 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0,
                    0.0, 0.0, 0.0, 1.0);
}

float4x4 MakeScaleMat(float3 scale) {
	float4x4 result = MakeUnit();
	result[0][0] = scale.x;
	result[1][1] = scale.y;
	result[2][2] = scale.z;
	result[3][3] = 1;
	return result;
}

float4x4 MakeTranslateMat(float3 translate) {
	float4x4 result = MakeUnit();
	result[0][0] = 1;
	result[1][1] = 1;
	result[2][2] = 1;
	result[3][0] = translate.x;
	result[3][1] = translate.y;
	result[3][2] = translate.z;
	result[3][3] = 1;
	return result;
}

float4x4 MakeRollMatrix(float z_Roll) {
	float4x4 result = MakeUnit();
	result[0][0] = cos(z_Roll);
	result[0][1] = sin(z_Roll);
	result[1][0] = -sin(z_Roll);
	result[1][1] = cos(z_Roll);
	result[2][2] = 1;
	result[3][3] = 1;
	return result;
}

float4x4 MakePitchMatrix(float x_Picth) {
	float4x4 result = MakeUnit();
	result[0][0] = 1;
	result[1][1] = cos(x_Picth);
	result[1][2] = sin(x_Picth);
	result[2][1] = -sin(x_Picth);
	result[2][2] = cos(x_Picth);
	result[3][3] = 1;
	return result;
}

float4x4 MakeYawMatrix(float y_Yaw) {
	float4x4 result = MakeUnit();
	result[0][0] = cos(y_Yaw);
	result[0][2] = -sin(y_Yaw);
	result[1][1] = 1;
	result[2][0] = sin(y_Yaw);
	result[2][2] = cos(y_Yaw);
	result[3][3] = 1;
	return result;
}

float4x4 MakeRotateMat(float3 rotate) {
	float4x4 result = MakeUnit();
	result = mul(mul(MakePitchMatrix(rotate.x), MakeYawMatrix(rotate.y)), MakeRollMatrix(rotate.z));
	return result;
}