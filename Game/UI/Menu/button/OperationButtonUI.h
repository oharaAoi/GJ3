#pragma once

#include "Game/UI/Menu/button/IMenuButtonUI.h"

class OperationButtonUI :
	public IMenuButtonUI
{
public:

	OperationButtonUI() = default;
	~OperationButtonUI() = default;

	void Init()override;

	void Update()override;

	void Debug_Gui()override;

private:

};
