#pragma once

#include "Game/UI/Button/IButtonUI.h"

class SelectButtonUI : 
	public IButtonUI
{
public:

	SelectButtonUI() = default;
	~SelectButtonUI() = default;

	void Init()override;

	void Update()override;

	void Debug_Gui()override;

private:


};

