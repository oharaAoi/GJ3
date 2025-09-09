#pragma once
#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

/// <summary>
/// 背景
/// </summary>
class BackGround :
	public AttributeGui {
public:

	BackGround() = default;
	~BackGround() = default;

	void Init(Canvas2d* _canvas2d);

	void Update();

	void Debug_Gui() override;

	void SetTexture(const std::string name) { sprite_->ReSetTexture(name); }

public:

	Sprite* GetSprite() { return sprite_; }

private:

	Sprite* sprite_;

};

