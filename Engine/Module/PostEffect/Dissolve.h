#pragma once
#include "Engine/Module/PostEffect/IPostEffect.h"
#include "Engine/Lib/Math/Vector4.h"
#include "Engine/Lib/Math/MathStructures.h"

class Dissolve :
	public IPostEffect {
public:

	struct DissolveSetting {
		Matrix4x4 uvTransform;
		Vector4 color;
		Vector4 edgeColor;
		float threshold = 0.5f;
	};

	struct DissolveParam : public IJsonConverter {
		SRT uvTransform;
		Vector4 color;
		Vector4 edgeColor;
		float threshold = 0.5f;
		bool isEnable;

		DissolveParam() { SetName("Dissolve"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("uvScale", uvTransform.scale)
				.Add("uvRptate", uvTransform.rotate)
				.Add("uvTranslate", uvTransform.translate)
				.Add("color", color)
				.Add("edgeColor", edgeColor)
				.Add("threshold", threshold)
				.Add("isEnable", isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "uvScale", uvTransform.scale);
			fromJson(jsonData, "uvRptate", uvTransform.rotate);
			fromJson(jsonData, "uvTranslate", uvTransform.translate);
			fromJson(jsonData, "color", color);
			fromJson(jsonData, "edgeColor", edgeColor);
			fromJson(jsonData, "threshold", threshold);
			fromJson(jsonData, "isEnable", isEnable);
		}
	};

public:

	Dissolve() = default;
	~Dissolve() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

private:

	std::unique_ptr<DxResource> settingBuffer_;
	DissolveSetting* setting_;

	DissolveParam param_;
	SRT uvTransform_;
};

