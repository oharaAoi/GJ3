#include "Particle.hlsli"

struct PerView {
	float4x4 viewProjection;
	float4x4 viewProjection2d;
	float4x4 billboardMat;
};

StructuredBuffer<CpuParticle> gParticles : register(t0);
ConstantBuffer<PerView> gPerView : register(b0);

struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID) {
	VertexShaderOutput output;
	
	CpuParticle particle = gParticles[instanceId];
	if (particle.draw2d == 0) {
		output.position = mul(input.position, mul(particle.worldMat, gPerView.viewProjection));
	} else {
		float4x4 wvp2d = mul(particle.worldMat, gPerView.viewProjection2d);
		output.position = mul(input.position, wvp2d);
	}
	output.texcoord = input.texcoord;
	output.color = particle.color;
	output.discardValue = particle.discardValue;
	
	return output;
}