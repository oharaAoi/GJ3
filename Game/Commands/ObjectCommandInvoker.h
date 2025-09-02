#pragma once

/// container
#include <list>
#include <vector>
#include <queue>
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
	/// <summary>
	/// 積まれた コマンドを実行する
	/// </summary>
	void ExecuteCommandRequest();

	void UndoCommand();
	void RedoCommand();

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
	std::list<std::unique_ptr<FrameCommand>> commandHistory_;
	std::list<std::unique_ptr<FrameCommand>>::iterator currentCommandItr_ = commandHistory_.end();

};
