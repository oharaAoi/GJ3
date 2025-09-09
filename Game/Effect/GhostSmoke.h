#pragma once
#include <memory>
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Module/Components/Animation/VectorTween.h"
#include "Engine/Lib/Json/IJsonConverter.h"
#include <Game/Effect/GhostEffect.h>

/// <summary>
/// Ghostのエフェクト単体を管理するクラス
/// </summary>
class GhostSmoke :
	public AttributeGui {
public:

	struct AnimationItems : public IJsonConverter {
		float appearDuration = 0;	// 出現時間
		float disappearDuration = 0;// 消失時間
		int appearEaseType = 0;		// イージングの種類
		int disappearEaseType = 0;	// イージングの種類

		AnimationItems() { SetName("AnimationItems"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("appearDuration", appearDuration)
				.Add("disappearDuration", disappearDuration)
				.Add("appearEaseType", appearEaseType)
				.Add("disappearEaseType", disappearEaseType)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "appearDuration", appearDuration);
			fromJson(jsonData, "disappearDuration", disappearDuration);
			fromJson(jsonData, "appearEaseType", appearEaseType);
			fromJson(jsonData, "disappearEaseType", disappearEaseType);
		}
	};

public:

	GhostSmoke() = default;
	~GhostSmoke();

	void Init(const Vector2& pos, const Vector2& tileSize);

	void Update();

	void Draw() const;

	void Debug_Gui() override;

public:

	void SetIsDestroy(bool _isDestroy) { isDestroy_ = _isDestroy; }
	bool GetIsDestroy() const { return isDestroy_; }

private:

	std::unique_ptr<GhostEffect> ghostEffect_[2];

	bool isDestroy_;

	VectorTween<float> appearanceAnime_;
	AnimationItems animationItems_;

};

