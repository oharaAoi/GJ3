#pragma once

/// Engine
#include "Engine/System/Input/Input.h"

///Player
class Player;
#include "Command/IPlayerCommand.h"

/// math
#include <stdint.h>

/// <summary>
/// プレイヤーを入力処理によって操作するクラス
/// 入力処理で コマンドを生成し, Invokerセットする
/// コマンドを実行するのは Invoker であって このクラスではない
/// </summary>
class PlayerInputHandler{
	/// 移動 キー
	static constexpr std::array<uint8_t,2> kMoveLeftKey = {DIK_LEFT,DIK_A};
	static constexpr std::array<uint8_t,2> kMoveRightKey = {DIK_RIGHT,DIK_D};
	static constexpr std::array<uint8_t,2> kMoveUpKey = {DIK_UP,DIK_W};
	static constexpr std::array<uint8_t,2> kMoveDownKey = {DIK_DOWN,DIK_S};

	static constexpr XInputButtons kMoveLeftButton = XInputButtons::DPAD_LEFT;
	static constexpr XInputButtons kMoveRightButton = XInputButtons::DPAD_RIGHT;
	static constexpr XInputButtons kMoveUpButton = XInputButtons::DPAD_UP;
	static constexpr XInputButtons kMoveDownButton = XInputButtons::DPAD_DOWN;

public:
	PlayerInputHandler();
	~PlayerInputHandler();

	void HandleInput();
protected:
	enum class MoveDirection{
		NONE, // 移動しない
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
private:
	Vector2Int DecideMoveDirection();
private:
	Player* player_ = nullptr;

	MoveDirection preMoveDirection = MoveDirection::NONE; // 前回の移動方向
	float autoMoveStartDelay   = 0.4f;  // 自動移動開始までの遅延
	float autoMoveStepInterval = 0.1f; // 自動移動の間隔
	float leftMoveEventTime_ = 0.0f; // 移動までの残り時間
public:
	void SetPlayer(Player* player){ player_ = player; }
};

