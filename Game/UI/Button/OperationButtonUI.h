#pragma once

#include "Game/UI/Button/IButtonUI.h"

class OperationButtonUI :
	public IButtonUI
{
public:

	OperationButtonUI() = default;
	~OperationButtonUI() = default;

	void Init()override;

	void Update()override;

	void Debug_Gui()override;

private:

};
