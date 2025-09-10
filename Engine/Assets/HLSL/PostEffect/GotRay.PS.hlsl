#include "ProcessedScene.hlsli"

struct GotRayParam {
	float4 color;
	float2 pos;
	float angle; // 光の角度
	float spread; // 光の広がり
	float cutoff; // 光筋のカットオフ
	float falloff; // 下方向へのフェード
	float edgeFade; // 左右端のフェード
	float speed;
	float ray1Density;
	float ray2Density;
	float ray3Density;
	float seed;
	float time;
};

Texture2D g_SceneTex : register(t0); // シーン全体のスクリーンテクスチャ
SamplerState g_Sampler : register(s0);
ConstantBuffer<GotRayParam> gGotRay : register(b0);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

float random(float2 uv) {
	return frac(sin(dot(uv.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

float noise(float2 uv) {
	float2 i = floor(uv);
	float2 f = frac(uv);

	float a = random(i);
	float b = random(i + float2(1.0, 0.0));
	float c = random(i + float2(0.0, 1.0));
	float d = random(i + float2(1.0, 1.0));

	float2 u = f * f * (3.0 - 2.0 * f);

	return lerp(a, b, u.x) +
           (c - a) * u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}

// 回転行列をUVに適用
float2 rotateUV(float2 uv, float ang) {
	float s = sin(ang);
	float c = cos(ang);
	return mul(uv, float2x2(c, -s, s, c));
}

// Screen blend
float3 ScreenBlend(float3 base, float3 blend) {
	return 1.0 - (1.0 - base) * (1.0 - blend);
}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float2 uv = input.texcoord;

    // Rotate, skew, move UVs
	float2 transformed = rotateUV(uv - gGotRay.pos, gGotRay.angle);
	transformed /= ((uv.y + gGotRay.spread) - (uv.y * gGotRay.spread));

    // Ray coords
	float2 ray1 = float2(transformed.x * gGotRay.ray1Density +
                         sin(gGotRay.time * 0.1 * gGotRay.speed) * (gGotRay.ray1Density * 0.2) + gGotRay.seed, 1.0);
	float2 ray2 = float2(transformed.x * gGotRay.ray2Density +
                         sin(gGotRay.time * 0.5 * gGotRay.speed) * (gGotRay.ray2Density * 0.2) + gGotRay.seed, 1.0);

    // Cutoff
	float cut = step(gGotRay.cutoff, transformed.x) * step(gGotRay.cutoff, 1.0 - transformed.x);
	ray1 *= cut;
	ray2 *= cut;

    // Noise rays
	float rays = 0.0;
	rays = saturate(noise(ray1) + noise(ray2) * 0.8f);
	    
    // Fade out edges
	rays *= smoothstep(0.0, gGotRay.falloff, (1.0 - uv.y)); // Bottom
	rays *= smoothstep(gGotRay.cutoff, gGotRay.edgeFade + gGotRay.cutoff, transformed.x); // Left
	rays *= smoothstep(gGotRay.cutoff, gGotRay.edgeFade + gGotRay.cutoff, 1.0 - transformed.x); // Right

    // Sample scene
	float3 sceneCol = g_SceneTex.Sample(g_Sampler, uv).rgb;

    // Rays color
	float3 rayCol = rays * gGotRay.color.rgb;

    // Blend with scene (Screen)
	float3 finalCol = ScreenBlend(sceneCol, rayCol);

	output.color.rgb = finalCol;
	output.color.a = 1.0f;

	return output;
}