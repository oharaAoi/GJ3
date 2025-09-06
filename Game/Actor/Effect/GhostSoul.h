#pragma once
#include "Engine/Module/Components/2d/BaseEntity2d.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

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

private:

	float easeT_ = 0.2f;
	int easeType_ = 0;

};