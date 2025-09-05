#pragma once
#include <string>
#include "Engine/Module/PostEffect/IPostEffect.h"
#include "Engine/Lib/Math/MyMatrix.h"
#include "Engine/Lib/Math/MathStructures.h"

class DistortionEffect :
	public IPostEffect {
public:

	struct DistortionEffectSetting {
		Matrix4x4 uv;
		float bias;
		float strength;
	};

	struct DistortionEffectParam : public IJsonConverter {
		SRT uvTransform;
		float bias;
		float strength;
		std::string noiseTextureName;
		bool isEnable;

		DistortionEffectParam() { SetName("DistortionEffect"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("uvScale", uvTransform.scale)
				.Add("uvRptate", uvTransform.rotate)
				.Add("uvTranslate", uvTransform.translate)
				.Add("bias", bias)
				.Add("strength", strength)
				.Add("noiseTextureName", noiseTextureName)
				.Add("isEnable", isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "uvScale", uvTransform.scale);
			fromJson(jsonData, "uvRptate", uvTransform.rotate);
			fromJson(jsonData, "uvTranslate", uvTransform.translate);
			fromJson(jsonData, "bias", bias);
			fromJson(jsonData, "strength", strength);
			fromJson(jsonData, "noiseTextureName", noiseTextureName);
			fromJson(jsonData, "isEnable", isEnable);
		}
	};

public:

	DistortionEffect() = default;
	~DistortionEffect() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

private:

	std::unique_ptr<DxResource> settingBuffer_;
	DistortionEffectSetting* setting_;

	SRT uvTransform_;
	std::string noiseTextureName_;

	DistortionEffectParam param_;

};

