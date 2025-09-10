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
		float pad[3];
	};

	struct GotRayParam : public IJsonConverter {
		Vector4 color = { 1,1,1,1 };
		Vector2 pos = { 0,0 };
		float angle = 1.f;	// 光の角度
		float spread = 1.f;	// 光の広がり
		float cutoff = 0.5f;	// 光筋のカットオフ
		float falloff = 0.5f;	// 下方向へのフェード
		float edgeFade = 1.f;	// 左右端のフェード
		float speed = 0;
		float ray1Density = 1.f;
		float ray2Density = 1.f;
		float ray3Density = 1.f;
		float seed = 0;
		float time = 0;
		bool isEnable = false;

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

public:

	void SetColor(const Vector4& _color) { param_.color = _color; }
	const Vector4& GetColor() const { return param_.color; }

	void SetPos(const Vector2& _pos) { param_.pos = _pos; }
	const Vector2& GetPos() const { return param_.pos; }

	void SetAngle(float _angle) { param_.angle = _angle; }
	float GetAngle() const { return param_.angle; }

	void SetSpread(float _spread) { param_.spread = _spread; }
	float GetSpread() const { return param_.spread; }

	void SetCutoff(float _value) { param_.cutoff = _value; }
	float GetCutoff() const { return param_.cutoff; }

	void SetEdgeFade(float _value) { param_.edgeFade = _value; }
	float GetEdgeFade() const { return param_.edgeFade; }

	void SetSpeed(float _speed) { param_.speed = _speed; }
	float GetSpeed() const { return param_.speed; }

	void SetRayDensity(float _value, uint16_t index);
	float GetRayDensity(uint16_t index);

	void SetSeed(float _value) { param_.seed = _value; }
	float GetSeed() const { return param_.seed; }

private:

	std::unique_ptr<DxResource> cBuffer_;
	GotRaySetting* setting_;

	GotRayParam param_;
};

