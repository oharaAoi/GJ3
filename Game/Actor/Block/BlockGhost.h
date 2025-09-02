#pragma once
#include "Game/Actor/Block/IBlock.h"

class BlockGhost :
	public IBlock {
public:

	BlockGhost() = default;
	~BlockGhost() override = default;

	void Init() override;
	void Update() override;
	void Debug_Gui() override;

private:

	Vector2 ConvertIndexToPosition(const Vector2Int& _index);
};

