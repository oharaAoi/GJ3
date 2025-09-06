#pragma once
#include <memory>
#include <vector>
// engine
#include "Engine/Module/Components/2d/Canvas2d.h"
// game
#include "Game/Actor/Effect/GhostSoul.h"
#include "Game/Actor/Player/Player.h"

/// <summary>
/// Ghostの魂を管理するクラス
/// </summary>
class GhostSoulManager{
public:

	GhostSoulManager() = default;
	~GhostSoulManager() = default;

	void Init(Canvas2d* _canvas2d,Player* _player);

	void Update();

	/// <summary>
	/// Soulの生成
	/// </summary>
	void CreateSoul(const Vector2& tileSize);

	void DeleteBackSoul(){ souls_.pop_back(); }

private:

	Canvas2d* pCanvas2d_;
	Player* pPlayer_;

	// object
	std::vector<std::unique_ptr<GhostSoul>> souls_;

};

