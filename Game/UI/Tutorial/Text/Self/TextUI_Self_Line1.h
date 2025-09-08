#pragma once

#include "Game/UI/Tutorial/Text/ITutorialTextUI.h"

class TextUI_Self_Line1 :
	public ITutorialTextUI
{
public:

	TextUI_Self_Line1() = default;
	~TextUI_Self_Line1() = default;

	void Init() override;

	void Update() override;

	void Debug_Gui() override;

private:



};

