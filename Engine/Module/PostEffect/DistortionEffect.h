#pragma once
#include <string>
#include "Engine/Module/PostEffect/IPostEffect.h"
#include "Engine/Lib/Math/MyMatrix.h"
#include "Engine/Lib/Math/MathStructures.h"

class DistortionEffect :
	public IPostEffect {
public:

	struct DistortionEffectParam {
		Matrix4x4 uv;
		float bias;
		float strength;
	};

public:

	DistortionEffect() = default;
	~DistortionEffect() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

private:

	std::unique_ptr<DxResource> settingBuffer_;
	DistortionEffectParam* setting_;

	SRT uvTransform_;
	std::string noiseTextureName_;

};

