#pragma once

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

#include "Game/UI/Button/ButtonType.h"

class IButtonUI : 
	public AttributeGui
{
public:

	IButtonUI() = default;
	virtual ~IButtonUI() = default;

	virtual void Init() = 0;

	virtual void Update() = 0;

	virtual void Debug_Gui() override = 0;

	// 点滅させる
	void Blinking();
	// 点滅リセット
	void Reset();
	// Sprite
	Sprite* GetSprite()const { return buttonUI_; }
	ButtonType GetButtonType()const { return type_; }

protected:

	// buttonSprite
	Sprite* buttonUI_ = nullptr;
	ButtonType type_ = ButtonType::Back;

	// 点滅時間と点滅間隔
	float blinkingTime_ = 2.0f;
	float blinkingFrame_ = 0.0f;

};

