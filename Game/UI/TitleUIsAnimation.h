#pragma once

#include "Engine/Module/Components/Attribute/AttributeGui.h"

class TitleUIs;

class TitleUIsAnimation :
	public AttributeGui
{
public:

	TitleUIsAnimation() = default;
	~TitleUIsAnimation() = default;

	void Init(TitleUIs* titleUIs);

	void Update();

	void Debug_Gui() override;

private:

	// 額縁が落ちる処理
	float DropDown(float time);


private:

	// タイトルのUI
	TitleUIs* titleUIs_ = nullptr;

	// 全体の尺
	float animaTimer_;

};

