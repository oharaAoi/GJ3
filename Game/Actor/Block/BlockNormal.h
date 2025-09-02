#pragma once
#include "Game/Actor/Block/IBlock.h"

class BlockNormal :
	public IBlock {
public:

	BlockNormal() = default;
	~BlockNormal() override = default;

	void Init() override;
	void Update() override;
	void Debug_Gui() override;
};

