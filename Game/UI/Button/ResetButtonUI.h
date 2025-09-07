#pragma once

#include "Game/UI/Button/IButtonUI.h"

class ResetButtonUI :
	public IButtonUI
{
public:

	ResetButtonUI() = default;
	~ResetButtonUI() = default;

	void Init()override;

	void Update()override;

	void Debug_Gui()override;

private:

};

