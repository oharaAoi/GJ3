#pragma once

#include "Game/UI/Tutorial/Text/ITutorialTextUI.h"

class TextUI_Move_Line3 :
	public ITutorialTextUI
{
public:

	TextUI_Move_Line3() = default;
	~TextUI_Move_Line3() = default;

	void Init() override;

	void Update() override;

	void Debug_Gui() override;

private:

};

