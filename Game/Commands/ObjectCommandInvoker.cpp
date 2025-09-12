#include "ObjectCommandInvoker.h"

/// System
#include "Engine/Lib/GameTimer.h"
#include "Engine/System/Input/Input.h"

void ObjectCommandInvoker::Initialize(){
	ClearHistory();
}
void ObjectCommandInvoker::Finalize(){
	ClearHistory();
}

UndoRedoState ObjectCommandInvoker::InputHandle(bool& _padIsInput,bool& _keyIsInput){
	// Undo Redo
	Input* input = Input::GetInstance();

	UndoRedoState currentAutoUndoRedoState = UndoRedoState::MANUAL;

	if(input->IsControllerConnected()){
		for(auto undoButton : kUndoButton){
			if(input->IsPressButton(undoButton)){
				currentAutoUndoRedoState = UndoRedoState::UNDO;

				_padIsInput = true;
				break;
			}
		}
		for(auto redoButton : kRedoButton){
			if(input->IsPressButton(redoButton)){
				currentAutoUndoRedoState = UndoRedoState::REDO;

				_padIsInput = true;
				break;
			}
		}
	}
	if(currentAutoUndoRedoState == UndoRedoState::MANUAL){
		for(auto redoKey : kRedoKey){
			if(input->IsPressKey(redoKey)){
				currentAutoUndoRedoState = UndoRedoState::REDO;

				_keyIsInput = true;
				break;
			}
		}
		for(auto undoKey : kUndoKey){
			if(input->IsPressKey(undoKey)){
				currentAutoUndoRedoState = UndoRedoState::UNDO;

				_keyIsInput = true;
				break;
			}
		}

	}

	// すべて Undo済み ならスキップ
	if(currentAutoUndoRedoState == UndoRedoState::UNDO){
		if(currentIndex_ == 0){
			return;
		}
	}
	// すべて Redo済み ならスキップ
	if(currentAutoUndoRedoState == UndoRedoState::REDO){
		if(currentIndex_ >= commandHistory_.size()){
			return;
		}
	}


	if(currentAutoUndoRedoState == UndoRedoState::MANUAL || currentAutoUndoRedoState != preAutoUndoRedoState_){
		isAutoUndoRedo_ = false;
	}

	leftUndoRedoDelay_ -= GameTimer::DeltaTime();
	if(leftUndoRedoDelay_ <= 0.f && currentAutoUndoRedoState != UndoRedoState::MANUAL){
		leftUndoRedoDelay_ = isAutoUndoRedo_ ? autoUndoRedoStepInterval_ : autoUndoRedoStartDelay_;
		isAutoUndoRedo_ = true;

		return currentAutoUndoRedoState;
	}

	return UndoRedoState::MANUAL;
}

void ObjectCommandInvoker::ExecuteCommandRequest(){

	if(commandRequests_.commandQueue_.empty()){
		return;
	}

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
