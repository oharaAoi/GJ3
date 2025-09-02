#include "ObjectCommandInvoker.h"

#include "Engine/System/Input/Input.h"

void ObjectCommandInvoker::Initialize(){
	ClearHistory();
}
void ObjectCommandInvoker::Finalize(){
	ClearHistory();
}

void ObjectCommandInvoker::Update(){
	///-------------------------------------------------------------------------------------------------
	// Commandの実行
	///-------------------------------------------------------------------------------------------------
	if(hasCommandRequest()){
		ExecuteCommandRequest();
	} else{
		// Undo Redo
		Input* input = Input::GetInstance();
		if(input->IsPressKey(DIK_LCONTROL)){
			if(input->IsPressKey(DIK_LSHIFT)){
				// SHIFT あり
				if(input->IsTriggerKey(DIK_Z)){
					RedoCommand();
				}
			} else{
				// SHIFT なし
				if(input->IsTriggerKey(DIK_Z)){
					UndoCommand();
				}
				if(input->IsTriggerKey(DIK_Y)){
					RedoCommand();
				}
			}
		}
	}
}

void ObjectCommandInvoker::ExecuteCommandRequest(){

	// 積まれたコマンドを全て実行する
	for(auto& cmd : commandRequests_.commandQueue_){
		cmd->Execute();
	}

	// 現在を指している履歴以降を削除
	if(currentIndex_ < commandHistory_.size()){
		commandHistory_.erase(commandHistory_.begin() + currentIndex_,commandHistory_.end());
	}

	// 新しい FrameCommand を履歴に push
	commandHistory_.push_back(std::move(commandRequests_));
	++currentIndex_;

	// 次のフレーム用に空の commandRequests_ を用意
	commandRequests_ = FrameCommand{};
}

void ObjectCommandInvoker::UndoCommand(){
	// すべて Undo済み ならスキップ
	if(currentIndex_ == 0){
		return;
	}

	// 逆順に Undo
	--currentIndex_;
	FrameCommand& frame = commandHistory_[currentIndex_];

	// 逆順に Undo
	for(auto it = frame.commandQueue_.rbegin(); it != frame.commandQueue_.rend(); ++it){
		(*it)->Undo();
	}
}

void ObjectCommandInvoker::RedoCommand(){
	// すべて Redo済み ならスキップ
	if(currentIndex_ >= commandHistory_.size()){
		return;
	}

	FrameCommand& frame = commandHistory_[currentIndex_];

	// 順番通りに再実行
	for(auto& cmd : frame.commandQueue_){
		cmd->Execute();
	}

	++currentIndex_;
}

void ObjectCommandInvoker::PushCommand(std::unique_ptr<IObjectCommand>&& command){
	commandRequests_.commandQueue_.emplace_back(std::move(command));
}

void ObjectCommandInvoker::ClearHistory(){
	commandHistory_.clear();
	currentIndex_ = 0;
	commandRequests_ = FrameCommand{};
}
