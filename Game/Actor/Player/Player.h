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
	const float moveAnimationTime_ = 0.2f; // アニメーションの時間
	float moveAnimationTimer_ = 0.0f; // アニメーションのタイマー
	bool isMoveAnimation_ = false;

	const float kPuniAnimationTime_ = 0.2f; // ぷにぷにアニメーションの時間
	float puniAnimationTimer_ = 0.0f; // ぷにぷにアニメーションのタイマー
	bool isPuniAnimation_ = false;

	SpriteDirection spriteDirection_ = SpriteDirection::RIGHT; // プレイヤーの向き
	Vector2 ConvertIndexToPosition(const Vector2Int& _index);	// 座標計算処理
	MapCollisionSystem* mapCollision_ = nullptr;
public:
	bool GetIsMoveAnimation() const{ return isMoveAnimation_; }
	void SetIsMoveAnimation(const bool flag){ isMoveAnimation_ = flag; }

	bool GetIsPuniAnimation() const{ return isPuniAnimation_; }
	void SetIsPuniAnimation(const bool flag){ isPuniAnimation_ = flag; }

	const SpriteDirection& GetSpriteDirection() const{ return spriteDirection_; }
	void SetSpriteDirection(const SpriteDirection& direction){ spriteDirection_ = direction; }

	bool isKeyInput() const;
	bool isPadInput() const;
};
