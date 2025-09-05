#pragma once

#include "Game/UI/Menu/button/IMenuButtonUI.h"

class SelectButtonUI : 
	public IMenuButtonUI
{
public:

	SelectButtonUI() = default;
	~SelectButtonUI() = default;

	void Init()override;

	void Update()override;

	void Debug_Gui()override;

private:


};

