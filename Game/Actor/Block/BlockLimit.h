#pragma once
#include "Game/Actor/Block/IBlock.h"

class BlockLimit :
	public IBlock {
public:

	BlockLimit() = default;
	~BlockLimit() override = default;

	void Init(Canvas2d* _canvas2d) override;
	void Update() override;
	void Debug_Gui() override;

private:

	Vector2 ConvertIndexToPosition(const Vector2Int& _index);
};

