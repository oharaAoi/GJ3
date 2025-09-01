#include "PBR.hlsli"
#include "Light.hlsli"

// 定数定義
static const float PI = 3.141592653589f;
#define EPSILON 1e-6;

struct Material{
	float4 color;
	int enableLighting;
	float4x4 uvTransform;
	float4 diffuseColor; // 色
	float4 specularColor;
	float roughness; // 粗さ
	float metallic;// 金属度
	float shininess;// 鋭さ
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<PointLight> gPointLight : register(b2);
ConstantBuffer<SpotLight> gSpotLight : register(b3);
Texture2D<float4> gTexture : register(t0);
Texture2D<float3> gNormapMap : register(t1);
//Texture2D<float> gMetallicMap : register(t2);
//Texture2D<float> gRoughnessMap : register(t3);

SamplerState gSampler : register(s0);
struct PixelShaderOutput{
	float4 color : SV_TARGET0;
};

//////////////////////////////////////////////////////////////
// 関数
//////////////////////////////////////////////////////////////

//==========================================
// Fresnel(Schlick)		F
//==========================================
float4 SchlickFresnel(float VDotH, float4 ks){
	float4 fresnel = (ks + (1.0f - ks) * pow((1.0f - VDotH), 5.0f));
	// textureColorfresnel = gMaterial.specularColor + (1.0f - gMaterial.specularColor) * pow((1.0f - VDotH), 5.0f);
	return fresnel;
}

//==========================================
// GGX					D
//==========================================
float GGX(float NDotH, float roughness){
	float denominator = (NDotH * NDotH) * (roughness - 1.0f) + 1.0f;
	
	return roughness / (PI * denominator * denominator);
}

//==========================================
// Height Correlated Smith		G
//==========================================
float HCSmith(float NdotV, float NdotL, float roughness){
	float NdotV2 = NdotV * NdotV;
	float NdotL2 = NdotL * NdotL;
	
	float Lambda_v = (-1.0 + sqrt(roughness * (1.0 - NdotL2) / NdotL2 + 1.0)) * 0.5f;
	float Lambda_l = (-1.0 + sqrt(roughness * (1.0 - NdotV2) / NdotV2 + 1.0)) * 0.5f;
	return 1.0 / (1.0 + Lambda_l + Lambda_v);
}

//==========================================
// BRDF(双方向反射率分布関数)
//==========================================
float4 BRDF(float NdotH, float NDotV, float NDotL, float VDotH, float4 ks, float roughness)
{
	float D = GGX(NdotH, roughness);
	float G = HCSmith(NDotV, NDotL, roughness);
	float4 F = SchlickFresnel(VDotH, ks);
	
	float4 brdf = (D*G*F) / (4 * NDotV * NDotL);
	
	brdf = saturate(brdf);
	
	return brdf;
}

//////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////
PixelShaderOutput main(VertexShaderOutput input){
	PixelShaderOutput output;
	
	//=======================================================
	// TextureのUVを求める
	//=======================================================
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	//=======================================================
	// 法線マップのタイリングを計算する
	//=======================================================
	float tilingFactor = 20.0; // タイリングを小さくするためのスケール（2.0でテクスチャが2倍のサイズに見える）
	float2 scaledUV = transformedUV.xy * tilingFactor; // UV座標をスケーリング
	float3 normalMap = gNormapMap.Sample(gSampler, scaledUV).xyz * 2.0 - 1.0;
	//float3 normal = normalize(mul(normalMap, input.tangentMat));
	float3 normal = normalize(input.normal);
	
	//=======================================================
	// ラフネスとメタリックのmapを計算する
	//=======================================================
	//float metallicMap = gMetallicMap.Sample(gSampler, transformedUV.xy).r;
	//float roughnessMap = gRoughnessMap.Sample(gSampler, transformedUV.xy).r;

	//float roughness = roughnessMap * roughnessMap + EPSILON;
	//float metallic = metallicMap;
	
	float roughness = gMaterial.roughness * gMaterial.roughness + EPSILON;
	float metallic = gMaterial.metallic;
	
	//=======================================================
	// 色を求める
	//=======================================================
	float4 F0 = lerp(float4(0.04, 0.04, 0.04, 1.0), gMaterial.color, metallic);
	float4 ks = F0; // 鏡面反射率はFresnel反射の基本値
	float4 kd = (1.0 - ks) * (1.0 - metallic); // 拡散反射率は非金属の部分のみ
	float4 diffuse = kd / PI; // Lambertian拡散反射
	
	//=======================================================
	// 内積などを求める
	//=======================================================
	float3 lightDir = normalize(-gDirectionalLight.direction);
	float3 viewDir = normalize(gDirectionalLight.eyePos - input.worldPos.xyz);
	float3 halfVec = normalize(viewDir + lightDir);
	float NdotH = saturate(dot(normal, halfVec));
	float NDotV = saturate(dot(normal, viewDir));
	float NDotL = saturate(dot(normal, lightDir));
	float VDotH = saturate(dot(viewDir, halfVec));
	
	diffuse.rgb = Lambert(NDotL, textureColor.rgb);
	
	//=======================================================
	// BRDF(双方向反射率分布関数)
	//=======================================================
	float4 brdf = BRDF(NdotH, NDotV, NDotL,VDotH, ks, roughness);
	
	//=======================================================
	 // 反射と拡散のバランスを取る
	float4 finalColor = brdf + diffuse + 0.3f;
	
	// レンダリング方程式の適用
	finalColor = finalColor * NDotL * gDirectionalLight.intensity;
	
	// テクスチャの色やライトの色を適応
	//output.color = finalColor * textureColor * gDirectionalLight.color;
	output.color.rgb = finalColor.rgb * textureColor.rgb * gDirectionalLight.color.rgb;
	output.color.a = gMaterial.color.a * textureColor.a;
	
	output.color = clamp(output.color, 0.0f, 1.0f);
	
	if (output.color.a <= 0.0f){
		discard;
	}

	return output;
}