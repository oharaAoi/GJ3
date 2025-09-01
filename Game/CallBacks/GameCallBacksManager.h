#pragma once
#include <memory>
#include "Engine/System/Manager/CollisionManager.h"

/// <summary>
/// CallBackをまとめたクラス
/// </summary>
class GameCallBacksManager {
public:

	GameCallBacksManager() = default;
	~GameCallBacksManager() = default;

	void Init(CollisionManager* _manager);

	/// <summary>
	/// 後で確実に消す
	/// </summary>
	void Update();

public:		// accessor method

private:

	// collisionManager
	CollisionManager* pCollisionManager_;

	// objectポインタ
	

};

