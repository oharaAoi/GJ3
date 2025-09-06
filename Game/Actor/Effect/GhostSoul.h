#pragma once
#include "Engine/Module/Components/2d/BaseEntity2d.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/Lib/Math/MyMath.h"

/// <summary>
/// Playerが獲得した魂
/// </summary>
class GhostSoul :
	public BaseEntity2d {
public:

	GhostSoul() = default;
	~GhostSoul() override {};

	void Init(Canvas2d* _canvas2d, const Vector2& _tileSize) ;

	void Update(const Vector2& _preIndexPos);

	void Debug_Gui() override;

	// Playerの周りを回転させる
	void RotatePlayer(const Vector2& _playerPos);

	void SetPosition(const Vector2& position) { sprite_->SetTranslate(position); }
	void SetTextureSize(const Vector2& size) { sprite_->ReSetTextureSize(size); }

private:

	float easeT_ = 0.3f;
	int easeType_ = 0;

	// 回転処理
	float angleRad_ = 0;
	float angleSpeed_ = 120.f * kToRadian;

};