#pragma once
#include "Game/Actor/Block/IBlock.h"

class BlockGrave : 
	public IBlock {
public:

	BlockGrave() = default;
	~BlockGrave() override = default;

	void Init() override;

	void Update() override;

	void Debug_Gui() override;

private:

};

