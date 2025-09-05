#pragma once

#include "Game/UI/Menu/button/IMenuButtonUI.h"

class ResetButtonUI :
	public IMenuButtonUI
{
public:

	ResetButtonUI() = default;
	~ResetButtonUI() = default;

	void Init()override;

	void Update()override;

	void Debug_Gui()override;

private:

};

