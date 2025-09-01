#pragma once
#include <cassert>
#include <string>
#include <map>
#include "Engine/DirectX/Utilities/DirectXUtils.h"

class Shader {
public:

	// 
	enum ShaderName {
		Normal,
		NormalEnviroment,
		TextureLess,
		Primitive,
		Phong,
		PBR,
		Particle,
		Skybox,
		Sprite,
		ObjectTextureBlend,
		ProcessedScene,
		Grayscale,
		RadialBlur,
		GlitchNoise,
		Vignette,
	};

	enum CsShaderName {
		GrayScale,
		HorizontalBlur,
		VerticalBlur,
		DepthOfField,
		SkinningCS,
		Blend,
		Result,

		GpuParticleInit,
		GpuParticleUpdate,
		GpuParticleEnd,
		EmitGpuParticle,
	};

	// 構造体
	struct ShaderData final {
		std::string vsShader;
		std::string psShader;
	};

public:

	Shader();
	~Shader();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	void Load(const std::string& vsPath, const std::string& psPath, const ShaderName& type);

	void Load(const std::string& csPath, const CsShaderName& type);

	const ShaderData& GetShaderData(const ShaderName& name){
		return shaderData_[name];
	}

	const std::string& GetCsShaderData(const CsShaderName& name) {
		return csShaderData_[name];
	}

private:

	// 名前を保存
	std::map<ShaderName, ShaderData> shaderData_;

	std::map<CsShaderName, std::string> csShaderData_;

};

