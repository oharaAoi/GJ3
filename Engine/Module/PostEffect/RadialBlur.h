#pragma once
#include "Engine/Lib/Math/Vector2.h"
#include "Engine/Module/PostEffect/IPostEffect.h"

/// <summary>
/// 放射ブラー
/// </summary>
class RadialBlur :
	public IPostEffect {
public:

	struct BlurSetting {
		Vector2 blurCenter = {0.5f, 0.5f};	// 放射ブラーの中心（通常はfloat2(0.5, 0.5)）
		float blurStrength = 0.00f;			// ブラーの強度（例: 0.02）
		float blurStart = 0.2f;				// ブラーの開始距離
		int sampleCount = 16;				// サンプル数（例: 16）
	};

	struct RadialBlurParam : public IJsonConverter {
		Vector2 blurCenter;
		float blurStrength = 0.00f;	
		float blurStart = 0.2f;	
		int sampleCount = 16;
		bool isEnable;

		RadialBlurParam() { SetName("RadialBlur"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("blurCenter", blurCenter)
				.Add("blurStrength", blurStrength)
				.Add("blurStart", blurStart)
				.Add("sampleCount", sampleCount)
				.Add("isEnable", isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "blurCenter", blurCenter);
			fromJson(jsonData, "blurStrength", blurStrength);
			fromJson(jsonData, "blurStart", blurStart);
			fromJson(jsonData, "sampleCount", blurStart);
			fromJson(jsonData, "isEnable", isEnable);
		}
	};

public:

	RadialBlur() = default;
	~RadialBlur() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

public:

	void SetStrength(float strength) { setting_->blurStrength = strength; }

	void Start(float strength, float startTime);

	void Stop(float stopTime);

private:

	std::unique_ptr<DxResource> blurSettingBuffer_;
	BlurSetting* setting_;

	bool start_;
	bool stop_;

	float targetStrength_;
	float preStrength_;

	float startTime_;
	float stopTime_;
	float timer_;

	RadialBlurParam param_;
};

