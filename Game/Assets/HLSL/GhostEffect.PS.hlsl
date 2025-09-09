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

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DissolveBuffer> gDissolve : register(b1);
Texture2D<float4> gBaseTexture : register(t0);
Texture2D<float> gDissolveTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gBaseTexture.Sample(gSampler, transformedUV.xy);
	
	// -----------------------------
	// ディゾルブの計算
	// -----------------------------
	
	float4 dissolveUV = mul(float4(input.texcoord, 0.0f, 1.0f), gDissolve.uvTransform);
	// maskを計算する
	float mask = gDissolveTexture.Sample(gSampler, dissolveUV.xy);
	// 閾値以下だったら透明にする
	if (mask <= gDissolve.threshold) {
		discard;
	}
	// edge部分の検出
	float edge = 1.0f - smoothstep(gDissolve.threshold, gDissolve.threshold + 0.03f, mask);
	
	// -----------------------------
	// 下のTextureの描画
	// -----------------------------
	
	output.color = gBaseTexture.Sample(gSampler, input.texcoord);
	// materialの色と合成
	output.color *= gDissolve.color;
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