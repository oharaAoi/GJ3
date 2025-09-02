#pragma once
#include "Engine/Module/Components/2d/Sprite.h"

/// <summary>
/// 背景
/// </summary>
class BackGround {
public:

	BackGround() = default;
	~BackGround() = default;

	void Init();

	void Update();

private:

	Sprite* sprite_;

};

