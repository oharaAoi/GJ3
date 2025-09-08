#pragma once

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

class ITutorialTextUI :
	public AttributeGui
{
public:

	ITutorialTextUI() = default;
	virtual ~ITutorialTextUI() = default;

	virtual void Init() = 0;

	virtual void Update() = 0;

	virtual void Debug_Gui() override = 0;

	Sprite* GetSprite()const { return textUI_; }
	void SetIsFinished(bool flag) { isFinished_ = flag; }
	bool GetIsFinished() const { return isFinished_; }
	void SetUvMinSize(float y) { textUI_->SetUvMinSize(Vector2{ 0.0f,y }); }
	void SetUvMaxSize(float x) { textUI_->SetUvMaxSize(Vector2{ x,1.0f }); }

protected:

	// TextSprite
	Sprite* textUI_ = nullptr;

	bool isFinished_ = false;

};

