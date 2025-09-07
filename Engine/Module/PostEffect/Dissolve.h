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
		Vector4 color = {1,1,1,1};
		Vector4 edgeColor = {1,1,1,1};
		float threshold = 0.5f;
		bool isEnable = false;

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

public:

	void SetUvScale(const Vector3& _scale) { uvTransform_.scale = _scale; }
	const Vector3& GetUvScale() const { return uvTransform_.scale; }

	void SetUvRotate(const Vector3& _rotate) { uvTransform_.rotate = _rotate; }
	const Vector3& GetUvRotate() const { return uvTransform_.rotate; }

	void SetUvTranslate(const Vector3& _translate) { uvTransform_.translate = _translate; }
	const Vector3& GetUvTranslate() const { return uvTransform_.translate; }

	void SetColor(const Vector4& _color) { param_.color = _color; }
	const Vector4& GetColor() const { return param_.color; }

	void SetEdgeColor(const Vector4& _color) { param_.edgeColor = _color; }
	const Vector4& GetEdgeColor() const { return param_.edgeColor; }

	void SetThreshold(float _threshold) { param_.threshold = _threshold; }
	float GetThreshold() const { return param_.threshold; }

private:

	std::unique_ptr<DxResource> settingBuffer_;
	DissolveSetting* setting_;

	DissolveParam param_;
	SRT uvTransform_;
};

