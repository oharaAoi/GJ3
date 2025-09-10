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

	struct Parameter : public IJsonConverter {
		Vector2 Title_su_Pos;
		Vector2 Title_ki_Pos;
		Vector2 Title_da_Pos;
		Vector2 Title_ma_Pos;
		Vector2 Title_ri_Pos;

		Parameter() { SetName("Title_TextUI"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("Title_su_Pos", Title_su_Pos)
				.Add("Title_ki_Pos", Title_ki_Pos)
				.Add("Title_da_Pos", Title_da_Pos)
				.Add("Title_ma_Pos", Title_ma_Pos)
				.Add("Title_ri_Pos", Title_ri_Pos)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "Title_su_Pos", Title_su_Pos);
			fromJson(jsonData, "Title_ki_Pos", Title_ki_Pos);
			fromJson(jsonData, "Title_da_Pos", Title_da_Pos);
			fromJson(jsonData, "Title_ma_Pos", Title_ma_Pos);
			fromJson(jsonData, "Title_ri_Pos", Title_ri_Pos);
		}
	};

public:

	TitleUIs() = default;
	~TitleUIs() = default;

	void Init();

	void Update();

	void Debug_Gui() override;

private:
	Sprite* backGround_;
	std::array<Sprite*,5> titleWords_;

	float uiAnimationTime_ = 0.7f;
	float uiAnimationTimer_ = 0.0f;
	float startAlpha_ = 0.2f;
	float endAlpha_ = 1.f;

	Sprite* spaceKey_;
	Sprite* aButton_;
	Sprite* shelf_;
	Sprite* ghost_;

	Parameter param_;
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
