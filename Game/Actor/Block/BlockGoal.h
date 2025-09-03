#pragma once
#include "Game/Actor/Block/IBlock.h"

class BlockGoal :
	public IBlock {
public:

	BlockGoal() = default;
	~BlockGoal() override = default;

	void Init(Canvas2d* _canvas2d) override;

	void Update() override;

	void Debug_Gui() override;

};

