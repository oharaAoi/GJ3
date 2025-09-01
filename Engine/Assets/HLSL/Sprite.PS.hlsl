#include "Sprite.hlsli"

struct Material {
	float4 color;
	float4x4 uvTransform;
	float2 uvMin;
	float2 uvMax;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	if (textureColor.a <= 0.01f) {
		discard;
	}
	
	if (gMaterial.color.a <= 0.01f) {
		discard;
	}
	
	if (transformedUV.x < gMaterial.uvMin.x) {
		discard;
	}
	
	if (transformedUV.y < gMaterial.uvMin.y) {
		discard;
	}
	
	if (transformedUV.x > gMaterial.uvMax.x) {
		discard;
	}
	
	if (transformedUV.y > gMaterial.uvMax.y) {
		discard;
	}
	
	output.color = textureColor * gMaterial.color;
	output.color = clamp(output.color, 0.0f, 1.0f);
	output.color.a = gMaterial.color.a * textureColor.a;
	
	return output;
}