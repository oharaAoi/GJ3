#pragma once

#include "Game/UI/Menu/button/IMenuButtonUI.h"

class BackButtonUI :
	public IMenuButtonUI
{
public:

	BackButtonUI() = default;
	~BackButtonUI() = default;

	void Init()override;

	void Update()override;

	void Debug_Gui()override;

private:

};

