#pragma once

#include "Game/UI/Tutorial/Text/ITutorialTextUI.h"

class TextUI_Move_Line2 :
	public ITutorialTextUI
{
public:

	TextUI_Move_Line2() = default;
	~TextUI_Move_Line2() = default;

	void Init() override;

	void Update() override;

	void Debug_Gui() override;

private:

};

