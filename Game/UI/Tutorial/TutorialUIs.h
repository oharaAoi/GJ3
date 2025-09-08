#pragma once
#include <memory>
#include <array>

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Lib/Json/IJsonConverter.h"

#include "Game/Information/TutorialType.h"
#include "Game/UI/Tutorial/Text/ITutorialTextUI.h"

class TutorialUIs :
	public AttributeGui
{
public:

	struct Parameter : public IJsonConverter {
		Vector2 buttonA_pos;
		Vector2 spaceKey_pos;
		Vector2 ghostBlock_pos;
		float ghostBlock_scale;
		Vector2 reset_R_pos;
		Vector2 reset_X_pos;

		Parameter() { SetName("TutorialUIs"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("buttonA_pos", buttonA_pos)
				.Add("spaceKey_pos", spaceKey_pos)
				.Add("ghostBlock_pos", ghostBlock_pos)
				.Add("ghostBlock_scale", ghostBlock_scale)
				.Add("reset_R_pos", reset_R_pos)
				.Add("reset_X_pos", reset_X_pos)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "buttonA_pos", buttonA_pos);
			fromJson(jsonData, "spaceKey_pos", spaceKey_pos);
			fromJson(jsonData, "ghostBlock_pos", ghostBlock_pos);
			fromJson(jsonData, "ghostBlock_scale", ghostBlock_scale);
			fromJson(jsonData, "reset_R_pos", reset_R_pos);
			fromJson(jsonData, "reset_X_pos", reset_X_pos);
		}
	};

public:

	TutorialUIs() = default;
	~TutorialUIs() = default;

	void Init();
	void Reset();
	void SpriteReset();

	void Update();

	void Debug_Gui() override;

	// 猫
	Sprite* GetCat()const { return cat_; }
	// 吹き出し
	Sprite* GetSpBubble()const { return speechBubble_; }
	// Aボタン
	Sprite* GetButtonA()const { return buttonA_; }
	// SPACEKey
	Sprite* GetSpaceKey()const { return spaceKey_; }
	// GhostBlock
	Sprite* GetGhostBlock()const { return ghostBlock_; }
	// GhostBlock
	Sprite* GetReset_R()const { return reset_R_; }
	// GhostBlock
	Sprite* GetReset_X()const { return reset_X_; }

	// 自己紹介しろ
	const std::array<std::unique_ptr<ITutorialTextUI>, 2>& GetSelfUIs()const { return selfUIs_; }
	// 移動を教える
	const std::array<std::unique_ptr<ITutorialTextUI>, 3>& GetModeUIs()const { return moveUIs_; }
	// ゴーストを作らせる
	const std::array<std::unique_ptr<ITutorialTextUI>, 2>& GetCreateUIs()const { return createUIs_; }
	// ゴーストが何故できるかの説明
	const std::array<std::unique_ptr<ITutorialTextUI>, 2>& GetExplanaUIs()const { return explanaUIs_; }
	// ゴーストを取らせる
	const std::array<std::unique_ptr<ITutorialTextUI>, 2>& GetCollisionUIs()const { return collisionUIs_; }
	// ゴールさせる
	const std::array<std::unique_ptr<ITutorialTextUI>, 2>& GetGoalUIs()const { return goalUIs_; }

private:


	Parameter param_;

	// チュートリアル猫
	Sprite* cat_ = nullptr;
	// チュートリアルの吹き出し
	Sprite* speechBubble_ = nullptr;
	// Aボタン
	Sprite* buttonA_ = nullptr;
	// SPACEキー
	Sprite* spaceKey_ = nullptr;
	// GhostBlock
	Sprite* ghostBlock_ = nullptr;
	// Resetボタン
	Sprite* reset_R_ = nullptr;
	Sprite* reset_X_ = nullptr;

	
	// チュートリアル説明用のText
	// 自己紹介しろ
	std::array<std::unique_ptr<ITutorialTextUI>, 2> selfUIs_;
	// 移動を教える
	std::array<std::unique_ptr<ITutorialTextUI>, 3> moveUIs_;
	// ゴーストを作らせる
	std::array<std::unique_ptr<ITutorialTextUI>, 2> createUIs_;
	// ゴーストが何故できるかの説明
	std::array<std::unique_ptr<ITutorialTextUI>, 2> explanaUIs_;
	// ゴーストを取らせる
	std::array<std::unique_ptr<ITutorialTextUI>, 2> collisionUIs_;
	// ゴールさせる
	std::array<std::unique_ptr<ITutorialTextUI>, 2> goalUIs_;

};

