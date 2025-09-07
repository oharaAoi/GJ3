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

	const float SwayMoveY();

	void SetPosition(const Vector2& position) { sprite_->SetTranslate(position); }
	void SetTextureSize(const Vector2& size) { 
		sprite_->ReSetTextureSize(size); 
		Canvas2d::ObjectPair* pair = pCanvas2d_->GetObjectPair(sprite_);
		pair->renderQueue = static_cast<int>(size.x);
		pair->isPreDraw = false;
	}


private:

	float easeT_ = 0.3f;
	int easeType_ = 0;

	// 回転処理
	float angleRad_ = 0;
	float angleSpeed_ = 120.f * kToRadian;
	// 縦揺れの処理
	float swayY_ = 0;

	Canvas2d* pCanvas2d_ = nullptr;

};