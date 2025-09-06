#pragma once

/// base
#include "Game/Actor/Block/IBlock.h"

///smart pointer
#include <memory>
#include <vector>

/// math
#include "Engine/Lib/Math/Vector2Int.h"

class PlayerInputHandler;
class MapCollisionSystem;

class Player
	:public IBlock{
public:
	Player();
	~Player();

	void Init(Canvas2d* _canvas2d);
	void Update() override;
	void Debug_Gui() override;

	MapCollisionSystem* GetMapCollision(){ return mapCollision_; }
	void SetMapCollision(MapCollisionSystem* collision){ mapCollision_ = collision; }

protected:
	std::unique_ptr<PlayerInputHandler> inputHandler_; // 入力ハンドラ
private:
	const float animationTime_ = 0.2f; // アニメーションの時間
	float animationTimer_ = 0.0f; // アニメーションのタイマー
	bool isAnimation_ = false;
	Vector2 ConvertIndexToPosition(const Vector2Int& _index);	// 座標計算処理

	MapCollisionSystem* mapCollision_ = nullptr;
public:
	const bool GetIsAnimation() const{ return isAnimation_; }
	void SetIsAnimation(const bool flag){ isAnimation_ = flag; }
};
