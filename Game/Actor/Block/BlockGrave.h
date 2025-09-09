#pragma once
#include "Game/Actor/Block/IBlock.h"
#include "Engine/Module/Components/Effect/BaseParticles.h"

class BlockGrave : 
	public IBlock {
public:

	BlockGrave() = default;
	~BlockGrave() override = default;

	void Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize) override;

	void Update() override;

	void Debug_Gui() override;

private:

	/// <summary>
	/// 落ちてくる演出
	/// </summary>
	void FallAppearance();

private:

	float timer_;			// 計測タイマー
	float fallTime_;		// 落下時間
	float startOffsetPosY_;	// 初期化時に+する値

	float startFallPosY_;	// 落下の初期座標
	float targetFallPosY_;	// 落下座標

	BaseParticles* smoke_;

};

