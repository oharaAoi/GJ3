#pragma once

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

class OperationUI :
	public AttributeGui
{
public:

	OperationUI() = default;
	~OperationUI() = default;

	void Init();

	void Update(bool openOperation);

	void Debug_Gui() override;

private:

	// メニュー背景
	Sprite* operation_ = nullptr;

	// フェード時間
	float fadeTime_ = 1.0f;
	float fadeFrame_ = 0.0f;

};

