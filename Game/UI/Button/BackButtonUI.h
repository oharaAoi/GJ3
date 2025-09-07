#pragma once

#include "Game/UI/Button/IButtonUI.h"

class BackButtonUI :
	public IButtonUI
{
public:

	BackButtonUI() = default;
	~BackButtonUI() = default;

	void Init()override;

	void Update()override;

	void Debug_Gui()override;

private:

};

