#pragma once
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Module/Components/2d/Sprite.h"

/// <summary>
/// 2dGameObjectの仲介
/// </summary>
class BaseEntity2d :
	public AttributeGui {
public:

	BaseEntity2d() = default;
	virtual ~BaseEntity2d() = default;

	virtual void Debug_Gui() override = 0;

public:	// accessor method

	void SetIsActive(bool _isActive) { isActive_ = _isActive; }
	bool GetIsActive() const { return isActive_; }

	void Destroy() { sprite_->SetIsDestroy(true); };

	Vector2 GetPosition() { return sprite_->GetTranslate(); }

	Sprite* GetSprite() const { return sprite_; }

	ScreenTransform* GetTransform() const { return transform_; }

protected:

	Sprite* sprite_;
	ScreenTransform* transform_;


};

