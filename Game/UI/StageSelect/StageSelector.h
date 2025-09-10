#pragma once

/// stl
#include <algorithm>
#include <array>
#include <memory>

/// engine
#include "Engine/Module/Components/Attribute/AttributeGui.h"

/// game
#include "Game/Actor/Block/BlockWall.h"
#include "Game/StageRender/StageRenderTarget.h"
#include "Game/UI/StageSelect/StageSelectCollection.h"
#include "Game/StageRender/StageContents.h"

/// math
#include <stdint.h>

class StageSelector
	:public AttributeGui {
private:
	static int32_t currentStageIndex_;

	static constexpr std::array<uint8_t, 2> kStageIndexSubKeys_ = { DIK_LEFT,DIK_A };
	static constexpr std::array<uint8_t, 2> kStageIndexAddKeys_ = { DIK_RIGHT,DIK_D };
	static constexpr std::array<uint8_t, 2> kStageDecideKeys_ = { DIK_RETURN,DIK_SPACE };
	static constexpr XInputButtons kStageIndexAddButtons_ = XInputButtons::DPAD_RIGHT;
	static constexpr XInputButtons kStageIndexSubButtons_ = XInputButtons::DPAD_LEFT;
	static constexpr std::array<XInputButtons, 2> kStageDecideButtons_ = { XInputButtons::BUTTON_A,XInputButtons::BUTTON_B };
public:
	static int32_t GetCurrentStageIndex() { return currentStageIndex_; }
	static void SetCurrentStageIndex(int32_t index) { currentStageIndex_ = index; }
public:
	StageSelector() = default;
	~StageSelector() = default;

	void Init();
	void Update();
	void Debug_Gui() override;

	void InputHandle();
	void Scroll();
private:
	struct RotateAnimationParam
		:public IJsonConverter {
	public:
		RotateAnimationParam();
		~RotateAnimationParam()override;

		void Init();
		void Update(Sprite* _sprite);

		int32_t CalculateEventIndex()const;

		json ToJson(const std::string& id) const override;
		void FromJson(const json& jsonData) override;

		float duration = 0.0f;
		float elapsedTime = 0.0f;

		/// pendulum
		Vector3 anchor = Vector3(0.f, 0.f, 0.f);
		float angle_ = 0.f;
		float angleVelo_ = 0.f;
		float gravity = 9.8f;
		float length = 32.f;

		int32_t currentAngleEventIndex_ = 0;
		int32_t preAngleEventIndex_ = -1;

		// 不規則に 動かす用
		AnimationCurve<float> angleEvent_;
	};


private:
	//Sprite* background_ = nullptr;
	std::array< Sprite*, 2> arrows_; // 矢印 0:L 1:R

	std::unique_ptr<RotateAnimationParam> leftArrowRotateParam_;
	std::unique_ptr<RotateAnimationParam> rightArrowRotateParam_;

	/// 押しっぱなし検出する時間
	float firstPressInterval_ = 0.4f;
	/// 押しっぱなし検出する時間（初回以降）
	float pressInterval_ = 0.1f;
	float leftPressTime_ = 0.0f;

	int32_t totalStageNum_ = 5; // 仮

	int32_t inputScrollDirection_ = 0; // -1:右へ +1:左へ
	int32_t scrollDirection_ = 0;      // -1:右へ +1:左へ

	float scrollT_ = 0.0f;          // 0~1でスクロールの進行度合い
	float currentIndexF_ = 0.0f;     // 入力に応じて更新される目標位置
	float scrollSpeed_ = 6.0f;      // インデックス/秒で移動

	bool isScrolling_ = false;
	bool decidedStage_ = false;

	Vector2 centerPos_ = Vector2(640.f,360.0f);
	float theSpaceBetweenButtons_ = 1280.0f;    // ボタン同士の間隔
	float currentOffsetX_ = 0.0f;   // 現在のオフセット位置
	float offsetY_ = -87.f; // Y オフセット Xは移動するが, Yは固定

	StageSelectCollection* pStageSelectCollection_;
	StageContents* pStageContents_;

	struct AnimaParam {
		float animaTimer;
		bool isAnima;
	};
	std::array<AnimaParam, 2> param_;

public:
	bool IsDecidedStage() const { return decidedStage_; }
	void SetDecidedStage(bool decided) { decidedStage_ = decided; }

	float GetLeftPressTime() const { return leftPressTime_; }
	int32_t GetScrollDirection() const { return scrollDirection_; }

	float GetScrollT() const { return scrollT_; }

	float GetCurrentOffsetX() const{ return currentOffsetX_; }

	int32_t GetTotalStageNum() const{ return totalStageNum_; }
	void SetTotalStageNum(int32_t num){ totalStageNum_ = num; }

	void SetStageSelectCollection(StageSelectCollection* _collection) { pStageSelectCollection_ = _collection; }
	void SetStageContents(StageContents* _contents) { pStageContents_ = _contents; }

private:

	float Radians(float deg) { return deg * kPI / 180.0f; }

	// 0..1 の正規化時間
	// 初回ピークのおおよその振れ幅（度）
	// 0..1 のあいだに何回 揺れるか（周回数）
	// 減衰の強さ（大きいほど早く収束）
	// 振り子のように揺れる処理
	float DampedPendulumAngle(
		float time,
		float amplitudeDeg = 25.0f,
		float cycles = 2.5f,
		float damping = 4.0f
	);


};