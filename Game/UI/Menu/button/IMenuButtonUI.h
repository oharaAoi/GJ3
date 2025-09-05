#pragma once

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

#include "Game/UI/Menu/button/MenuButtonType.h"

class IMenuButtonUI : 
	public AttributeGui
{
public:

	IMenuButtonUI() = default;
	virtual ~IMenuButtonUI() = default;

	virtual void Init() = 0;

	virtual void Update() = 0;

	virtual void Debug_Gui() override = 0;

	// 点滅させる
	void Blinking();
	// 点滅リセット
	void Reset();
	// Sprite
	Sprite* GetSprite()const { return buttonUI_; }
	MenuButtonType GetButtonType()const { return type_; }

protected:

	// buttonSprite
	Sprite* buttonUI_ = nullptr;
	MenuButtonType type_;

	// 点滅時間と点滅間隔
	float blinkingTime_ = 1.0f;
	float blinkingFrame_ = 0.0f;

};

