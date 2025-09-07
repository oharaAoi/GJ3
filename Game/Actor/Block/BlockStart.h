#pragma once
#include "Game/Actor/Block/IBlock.h"

class BlockStart :
	public IBlock {
public:

	BlockStart() = default;
	~BlockStart() override = default;

	void Init(Canvas2d* _canvas2d) override;

	void Update() override;

	void Debug_Gui() override;

private:
};

