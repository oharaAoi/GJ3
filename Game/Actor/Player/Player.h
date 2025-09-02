#pragma once

/// base
#include "Game/Actor/Block/IBlock.h"

///smart pointer
#include <memory>

/// math
#include "Engine/Lib/Math/Vector2Int.h"

class PlayerInputHandler;

class Player
	:public IBlock{
public:
	Player();
	~Player();

	void Init();
	void Update() override;
	void Debug_Gui() override;

protected:
	std::unique_ptr<PlayerInputHandler> inputHandler_; // 入力ハンドラ
public:
};

/// 一旦ここ
Vector2 ConvertIndexToPosition(const Vector2Int& _index, const Vector2& _size);