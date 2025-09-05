#pragma once

///stl
#include <array>

/// object
#include "Game/Manager/StageRegistry.h"

/// stl
#include <cstdint>

/// <summary>
/// Undo/Redo,Resetの入力を管理するクラス
/// </summary>
class StageInputHandler{
	static constexpr std::array<uint8_t,2> kUndoKey_ = {DIK_Q,DIK_Z};
	static constexpr std::array<XInputButtons,2> kUndoButton_ = {XInputButtons::LT_SHOULDER,XInputButtons::L_SHOULDER};
	static constexpr std::array<uint8_t,2> kRedoKey_ = {DIK_E,DIK_X};
	static constexpr std::array<XInputButtons,2> kRedoButton_ = {XInputButtons::RT_SHOULDER,XInputButtons::R_SHOULDER};

	static constexpr std::array<uint8_t,2> kResetKey_ = {DIK_R,DIK_Y};
	static constexpr std::array<XInputButtons,2> kResetButton_ = {XInputButtons::BUTTON_X,XInputButtons::BUTTON_Y};

public:
	static bool UndoInput();
	static bool RedoInput();
	static bool ResetInput();

};

