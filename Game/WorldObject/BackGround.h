#pragma once
#include "Engine/Module/Components/2d/Sprite.h"

/// <summary>
/// 背景
/// </summary>
class BackGround :
	public AttributeGui {
public:

	BackGround() = default;
	~BackGround() = default;

	void Init();

	void Update();

	void Debug_Gui() override;

	void SetTexture(const std::string name) { sprite_->ReSetTexture(name); }

private:

	Sprite* sprite_;

};

