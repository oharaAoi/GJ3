#pragma once

/// container
#include <array>
#include <deque>
#include <list>
#include <vector>

/// smart pointer
#include <memory>

///engine
#include "Engine/System/Input/Input.h"

/// command
#include "IObjectCommand.h"

/// <summary>
/// コマンドを実行するクラス
/// 基本は reqestQueue にコマンドを積んでおいて、ExecuteCommandRequest() で実行する
/// </summary>
class ObjectCommandInvoker{
	friend class UndoCommand;
	friend class RedoCommand;
public:
	static ObjectCommandInvoker& GetInstance(){
		static ObjectCommandInvoker instance;
		return instance;
	}

	static constexpr std::array<uint8_t,2> kUndoKey = {DIK_Z,DIK_Q};
	static constexpr std::array<uint8_t,2> kRedoKey = {DIK_Y,DIK_E};
	static constexpr  std::array<XInputButtons,2> kUndoButton = {XInputButtons::LT_SHOULDER,XInputButtons::L_SHOULDER};
	static constexpr  std::array<XInputButtons,2> kRedoButton = {XInputButtons::RT_SHOULDER,XInputButtons::R_SHOULDER};

public:
	void Initialize();
	void Finalize();

	void Update();

	/// <summary>
	/// 積まれた コマンドを実行する
	/// </summary>
	void ExecuteCommandRequest();

	void UndoCommand();
	void RedoCommand();

	void PushCommand(std::unique_ptr<IObjectCommand>&& command);

	void ClearHistory();
private:
	ObjectCommandInvoker() = default;
	ObjectCommandInvoker(const ObjectCommandInvoker&) = delete;
	ObjectCommandInvoker& operator=(const ObjectCommandInvoker&) = delete;
	~ObjectCommandInvoker() = default;

private:
	struct FrameCommand{
		std::vector<std::unique_ptr<IObjectCommand>> commandQueue_;
	};

	FrameCommand commandRequests_;
	std::deque<FrameCommand> commandHistory_;
	size_t currentIndex_ = 0;

	enum class UndoRedoState : int32_t{
		MANUAL = 0, // 何もしていない
		UNDO = 1, // Undo
		REDO = 2 // Redo
	};

	bool isAutoUndoRedo_ = false;
	UndoRedoState preAutoUndoRedoState_ = UndoRedoState::MANUAL; // 前回の状態

	float leftUndoRedoDelay_ = 0.0f; // Undo Redo の残り時間
	float autoUndoRedoStartDelay_ = 0.4f; // 自動 Undo Redo 開始までの遅延
	float autoUndoRedoStepInterval_ = 0.1f; // 自動 Undo Redo の間隔
public:
	bool hasCommandRequest()const{ return !commandRequests_.commandQueue_.empty(); }
};
