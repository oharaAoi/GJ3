#pragma once

/// base
#include "Engine/Module/Components/2d/BaseEntity2d.h"

/// math
#include "Engine/Lib/Math/Vector2Int.h"

class Player
	:public BaseEntity2d{
public:
	Player();
	~Player();

	void Init();

protected:
	Vector2Int index_ = Vector2Int(0,0); // 現在のインデックス

public:
	const Vector2Int& GetIndex()const{ return index_; }
	void SetIndex(const Vector2Int& _index){ index_ = _index; }
};

