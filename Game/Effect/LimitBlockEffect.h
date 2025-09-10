#pragma once
#include <memory>
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Module/Components/Animation/VectorTween.h"
#include "Engine/Lib/Json/IJsonConverter.h"
#include <Game/Effect/GhostEffect.h>

/// <summary>
/// 制限ブロックのエフェクト単体を管理するクラス
/// </summary>
class LimitBlockEffect :
	public AttributeGui {
public:

	struct UvScrollAnimationItems : public IJsonConverter {
		float effectDuration[2]{1.0f};
		
		UvScrollAnimationItems() { SetName("UvScrollAnimationItems"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("effect1Duration", effectDuration[0])
				.Add("effect2Duration", effectDuration[1])
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "effect1Duration", effectDuration[0]);
			fromJson(jsonData, "effect2Duration", effectDuration[1]);
		}
	};

public:

	LimitBlockEffect() = default;
	~LimitBlockEffect();

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

	std::unique_ptr<GhostEffect> ghostEffect_[2];

	bool isDestroy_;

	UvScrollAnimationItems animationItems_;
	VectorTween<Vector3> uvAnimation_[2];
	
	Vector2 position_;

};

