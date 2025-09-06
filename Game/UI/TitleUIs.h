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
	std::array<Sprite*,5> titleWords_;
	Sprite* spaceKey_;
	Sprite* shelf_;
	Sprite* ghost_;
public:
	Sprite* GetGhostSprite() const{ return ghost_; }
	Sprite* GetGhostSpriteRef(){ return ghost_; }

	const std::array<Sprite*,5>& GetTitleWords() const{ return titleWords_; }
	std::array<Sprite*,5>& GetTitleWordsRef(){ return titleWords_; }
	Sprite* GetSpaceKey() const{ return spaceKey_; }
	Sprite* GetSpaceKeyRef(){ return spaceKey_; }
	Sprite* GetShelf() const{ return shelf_; }
	Sprite* GetShelfRef(){ return shelf_; }
	Sprite* GetBackGround() const{ return backGround_; }
	Sprite* GetBackGroundRef(){ return backGround_; }
};
