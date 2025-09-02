#pragma once

/// stl
#include <algorithm>
#include <array>
#include <memory>

/// engine
#include "Engine/Module/Components/Attribute/AttributeGui.h"

/// game
#include "Game/Actor/Block/BlockWall.h"

/// math
#include <stdint.h>

class StageSelector
	:public AttributeGui{
private:
	static int32_t currentStageIndex_;

	static constexpr std::array<uint8_t,2> kStageIndexSubKeys_ = {DIK_LEFT,DIK_A};
	static constexpr std::array<uint8_t,2> kStageIndexAddKeys_ = {DIK_RIGHT,DIK_D};
	static constexpr std::array<uint8_t,2> kStageDecideKeys_ = {DIK_RETURN,DIK_SPACE};
public:
	static int32_t GetCurrentStageIndex(){ return currentStageIndex_; }
	static void SetCurrentStageIndex(int32_t index){ currentStageIndex_ = index; }
public:
	StageSelector() = default;
	~StageSelector() = default;

	void Init();
	void Update();
	void Debug_Gui() override;
	void ConvertIndexToScreen();

	void InputHandle();
	void Scroll();
private:
	std::array<std::unique_ptr<BlockWall>,3> stagePreviews_;

	/// 押しっぱなし検出する時間
	float firstPressInterval_ = 0.4f;
	/// 押しっぱなし検出する時間（初回以降）
	float pressInterval_ = 0.1f;
	float leftPressTime_ = 0.0f;

	int32_t totalStageNum_ = 5; // 仮
	int32_t targetStageIndex_ = 0; // スクロール先のインデックス
	int32_t scrollStartIndex_ = 0; // スクロール開始時のインデックス
	int32_t scrollDirection_ = 0;       // -1:左 / +1:右

	bool decidedStage_ = false;
	bool isScrolling_ = false;

	float scrollTime_ = 0.0f;       // 現在のスクロール時間
	float scrollDuration_ = 0.6f;   // スクロールにかける秒数
	Vector2 centerPos_ = Vector2(640.f,360.0f);
	float theSpaceBetweenButtons_ = 600.f;    // 描画用のオフセット（-1.0 ~ 1.0）
	float currentOffset_ = 0.0f;   // 現在のオフセット位置

public:
	bool IsDecidedStage() const{ return decidedStage_; }
	void SetDecidedStage(bool decided){ decidedStage_ = decided; }

	int32_t GetTotalStageNum() const{ return totalStageNum_; }
	void SetTotalStageNum(int32_t num){ totalStageNum_ = num; }
};