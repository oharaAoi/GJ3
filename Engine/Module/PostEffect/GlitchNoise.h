#pragma once
#include "Engine/Lib/Math/Vector2.h"
#include "Engine/Module/PostEffect/IPostEffect.h"

/// <summary>
/// グリッチエフェクト
/// </summary>
class GlitchNoise :
	public IPostEffect {
public:

	struct GlitchSetting {
		Vector2 texelSize;	// texelのサイズ
		float time;			// 時間
		float strength;
		int frameIndex;
	};

	struct GlitchNoiseParam : public IJsonConverter {
		Vector2 texelSize = { 1,1 };	// texelのサイズ
		float strength = 1.f;
		bool isEnable = false;

		GlitchNoiseParam() { SetName("GlitchNoise"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("texelSize", texelSize)
				.Add("strength", strength)
				.Add("isEnable", isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "texelSize", texelSize);
			fromJson(jsonData, "strength", strength);
			fromJson(jsonData, "isEnable", isEnable);
		}
	};

public:

	GlitchNoise() = default;
	~GlitchNoise() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void StartNoise(float startStrength, float time);

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

public:

	void SetTexelSize(const Vector2& _size) { param_.texelSize = _size; }
	const Vector2& GetTexelSize() const { return param_.texelSize; }

	void SetStrength(float _strength) { param_.strength = _strength; }
	float GetStrength() const { return param_.strength; }

private:

	std::unique_ptr<DxResource> glitchBuffer_;
	GlitchSetting* setting_;

	float noiseTime_;

	GlitchNoiseParam param_;
};

