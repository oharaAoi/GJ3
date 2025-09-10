#pragma once
#include <memory>
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Module/Components/Animation/VectorTween.h"
#include "Engine/Lib/Json/IJsonConverter.h"
#include <Game/Effect/GhostTakenSwirl.h>

/// <summary>
/// Ghostの取られた演出
/// </summary>
class GhostTakenEffect :
	public AttributeGui {
public:

	struct AnimationItems : public IJsonConverter {
		float duration = 10;

		AnimationItems() { SetName("GhostTakenAnimation"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("duration", duration)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "duration", duration);
		}
	};

public:

	GhostTakenEffect() = default;
	~GhostTakenEffect();

	void Init(const Vector2& pos, const Vector2& tileSize);

	void Update();

	void Draw() const;

	void Debug_Gui() override;

	void ApplySaveData(const std::string& effectName);

public:

	void SetIsDestroy(bool _isDestroy) { isDestroy_ = _isDestroy; }
	bool GetIsDestroy() const { return isDestroy_; }

	const Vector2& GetPos() const { return position_; }

private:

	std::unique_ptr<GhostTakenSwirl> ghostEffect_;
	bool isDestroy_;

	AnimationItems animationItems_;
	float lifeTimer_;

	Vector2 position_;

};