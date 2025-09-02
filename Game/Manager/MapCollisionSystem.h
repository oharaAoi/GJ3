#pragma once

#include "StageRegistry.h"

class MapCollisionSystem
{
public:

	void Init(StageRegistry* stageRegistry);

	void Update();


	// 進めるか判定する(進む方向、プレイヤー)
	bool IsMovable(const Vector2Int& direction, IBlock* player);


private:

	void ChengeStage(
		const Vector2Int& direction,
		IBlock* player,
		bool isSetBlock = false
	);

private:

	StageRegistry* stageRegistry_ = nullptr;

};

