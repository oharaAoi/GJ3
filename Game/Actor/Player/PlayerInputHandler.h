#pragma once

/// Engine
#include "Engine/System/Input/Input.h"

///Player
#include "Player.h"
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


public:
	PlayerInputHandler() = default;
	~PlayerInputHandler() = default;

	void HandleInput();
private:
	Vector2Int DecideMoveDirection();
private:
	Player* player_ = nullptr;
public:
	void SetPlayer(Player* player){ player_ = player; }
};

