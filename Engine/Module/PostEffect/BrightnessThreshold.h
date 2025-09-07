#pragma once
#include "Engine/Module/PostEffect/IPostEffect.h"

class BrightnessThreshold :
	public IPostEffect {
public:	// 構造体

	struct BloomSettings {
		float threshold = 1.0f;
	};

	struct BrightnessThresholdParam : public IJsonConverter {
		float threshold = 1.0f;
		bool isEnable = false;

		BrightnessThresholdParam() { SetName("BrightnessThresholdParam"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("threshold", threshold)
				.Add("isEnable", isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "threshold", threshold);
			fromJson(jsonData, "isEnable", isEnable);
		}
	};

public:

	BrightnessThreshold() = default;
	~BrightnessThreshold() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;
	void CopyData() override;

private:

	std::unique_ptr<DxResource> bloomBuffer_;
	BloomSettings* bloomSetting_;

	BrightnessThresholdParam param_;

public:
	float GetThreshold() const{ return param_.threshold; }
	void SetThreshold(float _threshold){ param_.threshold = _threshold; }
};

