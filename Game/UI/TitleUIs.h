#pragma once
#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

///stl
// container
#include <array>
#include <vector>

/// engine
// animation
#include "Engine/Module/Components/Animation/AnimationStructures.h"

/// math
#include <cstdint>


/// <summary>
/// TitleのUIをまとめたクラス
/// </summary>
class TitleUIs :
	public AttributeGui{
public:

	TitleUIs() = default;
	~TitleUIs() = default;

	void Init();

	void Update();

	void Debug_Gui() override;

private:
	Sprite* backGround_;
	Sprite* titleBar_;
	Sprite* shelf_;
	Sprite* ghost_;
public:
	Sprite* GetGhostSprite() const{ return ghost_; }
	Sprite* GetGhostSpriteRef(){ return ghost_; }

	Sprite* GetTitleBar() const{ return titleBar_; }
	Sprite* GetTitleBarRef(){ return titleBar_; }
	Sprite* GetShelf() const{ return shelf_; }
	Sprite* GetShelfRef(){ return shelf_; }
	Sprite* GetBackGround() const{ return backGround_; }
	Sprite* GetBackGroundRef(){ return backGround_; }
};
