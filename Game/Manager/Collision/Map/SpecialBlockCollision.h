#pragma once

#include "Engine/Lib/Math/Vector2Int.h"

class MapCollisionSystem;

class SpecialBlockCollision
{
public:

	void Init(MapCollisionSystem* system);


	// 特殊ブロックが変化するか判定する
	void ChangeBlock();
	// 特殊ブロックの変化を判定する再帰関数(ゴーストIndexを入れる)
	void RecursionBlockChecker(const Vector2Int& _index);

private:

	MapCollisionSystem* system_ = nullptr;

};

