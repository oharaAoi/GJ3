#pragma once
#include "Game/Actor/Block/IBlock.h"

class BlockGrave : 
	public IBlock {
public:

	BlockGrave() = default;
	~BlockGrave() override = default;

	void Init(Canvas2d* _canvas2d) override;

	void Update() override;

	void Debug_Gui() override;

private:

};

