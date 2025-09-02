#pragma once
#include "Game/Actor/Block/IBlock.h"

class BlockGoal :
	public IBlock {
public:

	BlockGoal() = default;
	~BlockGoal() override = default;

	void Init() override;

	void Update() override;

	void Debug_Gui() override;

};

