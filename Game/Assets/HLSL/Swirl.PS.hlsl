#include "ProcessedScene.hlsli"

struct Setting {
	float4x4 uv;
};

//ConstantBuffer<Setting> gSetting : register(b0);
Texture2D<float4> gSceenTexture : register(t0);
//Texture2D<float4> gPatternTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	output.color = gSceenTexture.Sample(gSampler, input.texcoord);
	
	return output;
}