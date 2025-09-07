#pragma once

#include "Game/UI/Button/IButtonUI.h"

class NextStageButtonUI :
	public IButtonUI
{
public:

	NextStageButtonUI() = default;
	~NextStageButtonUI() = default;

	void Init()override;

	void Update()override;

	void Debug_Gui()override;

private:


};

