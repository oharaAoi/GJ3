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
	Vector2Int index_ = Vector2Int(0,0); // 現在のインデックス

	std::unique_ptr<PlayerInputHandler> inputHandler_; // 入力ハンドラ
public:
	const Vector2Int& GetIndex()const{ return index_; }
	void SetIndex(const Vector2Int& _index){ index_ = _index; }
};

