#pragma once
#include "Game/Actor/Block/IBlock.h"

class BlockWall :
	public IBlock {
public:

	BlockWall() = default;
	~BlockWall() override = default;

	void Init() override;

	void Update() override;

	void Debug_Gui() override;

private:

};

