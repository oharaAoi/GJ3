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
	static constexpr std::array<const char*,2> kPlayerSideTextureName = {"player_left.png","player_right.png"};

	Player();
	~Player();

	void Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize);
	void Update() override;
	void Debug_Gui() override;

	MapCollisionSystem* GetMapCollision(){ return mapCollision_; }
	void SetMapCollision(MapCollisionSystem* collision){ mapCollision_ = collision; }

	enum class SpriteDirection{
		LEFT,
		RIGHT,
	};

protected:
	std::unique_ptr<PlayerInputHandler> inputHandler_; // 入力ハンドラ
private:
	const float animationTime_ = 0.2f; // アニメーションの時間
	float animationTimer_ = 0.0f; // アニメーションのタイマー
	bool isAnimation_ = false;
	SpriteDirection spriteDirection_ = SpriteDirection::RIGHT; // プレイヤーの向き
	Vector2 ConvertIndexToPosition(const Vector2Int& _index);	// 座標計算処理
	MapCollisionSystem* mapCollision_ = nullptr;
public:
	const bool GetIsAnimation() const{ return isAnimation_; }
	void SetIsAnimation(const bool flag){ isAnimation_ = flag; }

	const SpriteDirection& GetSpriteDirection() const{ return spriteDirection_; }
	void SetSpriteDirection(const SpriteDirection& direction){ spriteDirection_ = direction; }

	bool isKeyInput() const;
	bool isPadInput() const;
};
