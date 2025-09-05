#pragma once
#include "Engine/Lib/Math/Vector4.h"
#include "Engine/Module/PostEffect/IPostEffect.h"

class Grayscale :
	public IPostEffect {
public:

	struct Setting {
		Vector4 color;
	};
	
	struct GrayScaleParam : public IJsonConverter {
		Vector4 color;
		bool isEnable;

		GrayScaleParam() { SetName("GrayScale"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("color", color)
				.Add("isEnable", isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "color", color);
			fromJson(jsonData, "isEnable", isEnable);
		}
	};

public:

	Grayscale() = default;
	~Grayscale() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

public:

	void SetColor(const Vector4& _color) { setting_->color = _color; }

private:

	std::unique_ptr<DxResource> settingBuffer_;
	Setting* setting_;

	GrayScaleParam param_;

};

