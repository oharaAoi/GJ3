#pragma once
#include <string>
#include "Engine/Module/PostEffect/IPostEffect.h"
#include "Engine/Lib/Math/MyMatrix.h"
#include "Engine/Lib/Math/MathStructures.h"

/// <summary>
/// 2d画像を利用して行うGotRay
/// </summary>
class ScreenGotRay :
	public IPostEffect {
public:

	struct GotRaySetting {
		Vector4 color;
		Vector2 pos;
		float pad[2];
		float angle;	// 光の角度
		float spread;	// 光の広がり
		float cutoff;	// 光筋のカットオフ
		float falloff;	// 下方向へのフェード
		float edgeFade;	// 左右端のフェード
		float speed;
		float ray1Density;
		float ray2Density;
		float ray3Density;
		float seed;
		float time;
	};

	struct GotRayParam : public IJsonConverter {
		Vector4 color;
		Vector2 pos;
		float angle;	// 光の角度
		float spread;	// 光の広がり
		float cutoff;	// 光筋のカットオフ
		float falloff;	// 下方向へのフェード
		float edgeFade;	// 左右端のフェード
		float speed;
		float ray1Density;
		float ray2Density;
		float ray3Density;
		float seed;
		float time;
		bool isEnable;

		GotRayParam() { SetName("GotRay"); };

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("color", color)
				.Add("pos", pos)
				.Add("angle", angle)
				.Add("spread", spread)
				.Add("cutoff", cutoff)
				.Add("edgeFade", edgeFade)
				.Add("speed", speed)
				.Add("ray1Density", ray1Density)
				.Add("ray2Density", ray2Density)
				.Add("ray3Density", ray3Density)
				.Add("seed", seed)
				.Add("time", time)
				.Add("isEnable", isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "color", color);
			fromJson(jsonData, "pos", pos);
			fromJson(jsonData, "angle", angle);
			fromJson(jsonData, "spread", spread);
			fromJson(jsonData, "cutoff", cutoff);
			fromJson(jsonData, "edgeFade", edgeFade);
			fromJson(jsonData, "speed", speed);
			fromJson(jsonData, "ray1Density", ray1Density);
			fromJson(jsonData, "ray2Density", ray2Density);
			fromJson(jsonData, "ray3Density", ray3Density);
			fromJson(jsonData, "seed", seed);
			fromJson(jsonData, "isEnable", isEnable);
		}
	};

public:

	ScreenGotRay() = default;
	~ScreenGotRay() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

private:

	std::unique_ptr<DxResource> cBuffer_;
	GotRaySetting* setting_;

	GotRayParam param_;
};

