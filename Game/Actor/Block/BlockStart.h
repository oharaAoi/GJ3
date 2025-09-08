#pragma once
#include "Game/Actor/Block/IBlock.h"

class BlockStart :
	public IBlock {
public:

	BlockStart() = default;
	~BlockStart() override = default;

	void Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize) override;

	void Update() override;

	void Debug_Gui() override;

private:
};

