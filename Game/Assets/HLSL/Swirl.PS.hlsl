#include "ProcessedScene.hlsli"

static const float pi2 = 6.28318530718f;
static const float COLWID = 0.4f;

struct Setting {
	float2 screenSize;	// windowのサイズ
	float time;			// 時間
	float radiusKernel;	// 半径方向の支配
	float angleStrength;// 角度方向のねじれの強さ
	float speed;		// 速度
	float frontWidth;	// フロント幅(にじみ)
};
ConstantBuffer<Setting> gSetting : register(b0);
Texture2D<float4> gSceneTexture : register(t0);
Texture2D<float4> gPatternTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

float SmoothStep(float a, float b, float x) {
	float t = saturate((x - a) / (b - a));
	return t * t * (3.0f - 2.0f * t);
}
float Zigzag(float x) {
	return 1.0f - abs(1.0f - 2.0f * frac(x));
}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	
	float2 uv01 = input.texcoord;
	float2 uvFlip = uv01;
	float2 fragCoord = uvFlip * gSetting.screenSize;
	float2 uvR = 2.0f * fragCoord - gSetting.screenSize;
	float diagLength = length(gSetting.screenSize);
	float2 uv = uvR / max(diagLength, 1.0f);
	float4 sceneColor = gSceneTexture.Sample(gSampler, uv01);
	
	 // 渦の座標特徴量
	float r2 = max(dot(uv, uv), 1e-8f);
	float lenSq = log(r2); // 半径をログで圧縮
	float thetaN = atan2(uv.y, uv.x) / pi2; // -0.5..0.5 の角度
	
	float arrival = -gSetting.radiusKernel * lenSq - gSetting.angleStrength * thetaN;
	float progress = gSetting.time * gSetting.speed;
	
	float cover = SmoothStep(progress - gSetting.frontWidth, progress, arrival);
	
	float time01 = frac(gSetting.time / 4.0f);
	float pat = Zigzag(0.22f * lenSq + (-1.0f) * thetaN + 1.0f * time01) * COLWID
              + 0.05f * lenSq
              + 2.0f * time01;
	
	float u = frac(pat);
	uint texW, texH;
	gPatternTexture.GetDimensions(texW, texH);
	float4 swirlColor = gPatternTexture.Sample(gSampler, float2(u, 0.5f));
	
	float centerMask = 1.0f - cover; // 中央=1, 周囲=0
	float3 finalRgb = lerp(swirlColor.rgb, sceneColor.rgb, centerMask);
	if (length(swirlColor.rgb) >= 1.0f) {
		output.color = sceneColor;
	}
	else {
		output.color = float4(finalRgb, 1.0f);
	}
	
	//float2 uv01 = input.texcoord;
	//float2 screenSize = gSetting.screenSize;
	//float4 sceneColor = gSceneTexture.Sample(gSampler, uv01);
	
	//float2 fragCoord = uv01 * gSetting.screenSize;

	//// -----------------------------------------------
	//// 渦の計算を行う
	//// -----------------------------------------------
	//// 画面中心を原点に
	//float2 uvR = 2.0f * fragCoord - gSetting.screenSize;

	//// 画面の対角スケール
	//float diagLen = length(gSetting.screenSize);
	//float2 uv = uvR / max(diagLen, 1.0f);

	//// r^2 の対数
	//float r2 = max(dot(uv, uv), 1e-8f);
	//float lenSq = log(r2);

	//// 角度を -0.5..0.5 に正規化
	//float angle = atan2(uv.y, uv.x) / pi2;

	//// 0..1 に折りたたんだ time
	//float time = frac(gSetting.time / 4.0f);

	//float pat = Zigzag(0.1f * lenSq + (-1.0f) * angle + 1.0f * time) * COLWID
 //             + 0.05f * lenSq
 //             + 2.0f * time;

	//float u = frac(pat);

 //   // パレットテクスチャを横方向1D的に使用
	//uint texW, texH;
	//gPatternTexture.GetDimensions(texW, texH);
	//float v = 0.5f; // 高さ1の画像なら0.5fでOK
	//float4 swirlColor = gPatternTexture.Sample(gSampler, float2(u, v));
	
	//if (length(swirlColor.rgb) >= 1.0f) {
	//	output.color = sceneColor;
	//	return output;
	//}
	//else {
	//	output.color = float4(swirlColor.rgb, 1.0f);
	//}
	
	return output;
}