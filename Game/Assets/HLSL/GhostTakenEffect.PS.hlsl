#include "Sprite.hlsli"

struct Material {
	float4 color;
	float4x4 uvTransform;
};

struct DissolveBuffer {
	float4x4 uvTransform;
	float4 color;
	float4 edgeColor;
	float threshold;
};

struct SwirlParam {
	float2 center; // 渦の中心 (0~1, uv空間で指定)
	float swirlStrength; // 渦の強さ (正で右巻き, 負で左巻き)
	float time; // アニメーション回転 (不要なら0)
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DissolveBuffer> gDissolve : register(b1);
ConstantBuffer<SwirlParam> gSwirl : register(b2);
Texture2D<float4> gBaseTexture : register(t0);
Texture2D<float> gDissolveTexture : register(t1);
Texture2D<float> gMaskTexture : register(t2);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gBaseTexture.Sample(gSampler, transformedUV.xy);
	
	// -----------------------------
	// 渦の計算
	// -----------------------------
	
	// 中心を基準にした座標 (-1 ~ 1)
	float2 p = input.texcoord - gSwirl.center;

    // 極座標変換
	float r = length(p);
	float theta = atan2(p.y, p.x);

    // 渦巻き効果
	theta += gSwirl.swirlStrength * r + gSwirl.time * 0.5f;

    // 新しい座標
	float2 newUV;
	newUV.x = r * cos(theta);
	newUV.y = r * sin(theta);

    // UV空間に戻す
	newUV += gSwirl.center;
	
	// -----------------------------
	// ディゾルブの計算
	// -----------------------------
	
	float4 dissolveUV = mul(float4(input.texcoord, 0.0f, 1.0f), gDissolve.uvTransform);
	// maskを計算する
	float dissolveMask = gDissolveTexture.Sample(gSampler, newUV.xy);
	// 閾値以下だったら透明にする
	if (dissolveMask <= gDissolve.threshold) {
		discard;
	}
	// edge部分の検出
	float edge = 1.0f - smoothstep(gDissolve.threshold, gDissolve.threshold + 0.03f, dissolveMask);
	
	// -----------------------------
	// maskのサンプル
	// -----------------------------
	
	float mask = gMaskTexture.Sample(gSampler, input.texcoord).r;

	// -----------------------------
	// 最終画像の計算
	// -----------------------------
	
	output.color = gBaseTexture.Sample(gSampler, transformedUV.xy);
	output.color = lerp(output.color, gDissolve.color, mask);
	// edge部分ならedgeColorを足す
	output.color.rgb += edge * gDissolve.edgeColor.rgb;
	output.color.a = gMaterial.color.a * textureColor.a;
	output.color = clamp(output.color, 0.0f, 1.0f);
	
	if (textureColor.a <= 0.01f) {
		discard;
	}
	
	if (gMaterial.color.a <= 0.01f) {
		discard;
	}
	
	return output;
}