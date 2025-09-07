#pragma once

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

///stl
// container
#include <array>
#include <vector>

/// engine
// animation
#include "Engine/Module/Components/Animation/AnimationStructures.h"

/// math
#include <cstdint>

class ThunderFlash :
	public AttributeGui{
public:
	void Init();
	void Finalize();
	void Update();

	void Debug_Gui() override;
protected:
	float CalculateCurrentFlushAlpha()const;

	struct Parameter
		: public IJsonConverter{
		Parameter() = default;
		~Parameter() override = default;

		float currentFlashTime_ = 0.0f;
		float flashDuration_ = 10.0f; // 点滅の全体時間 (sec)
		AnimationCurve<float> flashCurve_;

		json ToJson(const std::string& id) const override;

		void FromJson(const json& jsonData) override;
	};

private:
	float alpha_ = 0.0f;

	Parameter parameter_;
public:
	float GetAlpha() const{ return alpha_; }
	void SetAlpha(float alpha){ alpha_ = alpha; }

};
class LightFlash :
	public AttributeGui{
public:
	void Init(const std::string& _parameterName);
	void Finalize();
	void Update();

	void Debug_Gui() override;
protected:
	float CalculateCurrentFlushAlpha()const;

	struct Parameter
		: public IJsonConverter{
		Parameter() = default;
		~Parameter() override = default;

		float currentFlashTime_ = 0.0f;
		float flashDuration_ = 10.0f; // 点滅の全体時間 (sec)
		AnimationCurve<float> flashCurve_;

		json ToJson(const std::string& id) const override;

		void FromJson(const json& jsonData) override;
	};

private:
	bool isFinish_ = false;
	// 点滅時に画面を覆う用のSprite
	Sprite* flashOverlaySprite_;

	std::string parameterName_;

	Parameter parameter_;
public:
	const Vector4& GetFlashColor() const{ return flashOverlaySprite_->GetColor(); }
	void SetFlashColor(const Vector4& color){ flashOverlaySprite_->SetColor(color); }

	void setCurrentFlashTime(float time){ parameter_.currentFlashTime_ = time; }

	bool GetIsFinish() const{ return isFinish_; }
};