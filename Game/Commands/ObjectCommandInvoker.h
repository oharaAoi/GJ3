#pragma once

/// container
#include <deque>
#include <list>
#include <vector>
/// smart pointer
#include <memory>

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
	static ObjectCommandInvoker& getInstance(){
		static ObjectCommandInvoker instance;
		return instance;
	}

public:
	void Initialize();
	void Finalize();

	/// <summary>
	/// 積まれた コマンドを実行する
	/// </summary>
	void ExecuteCommandRequest();

	void UndoCommand();
	void RedoCommand();

	void ClearHistory();
	void Reset();
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
};
