#include "ObjectCommandInvoker.h"

void ObjectCommandInvoker::ExecuteCommandRequest(){}
void ObjectCommandInvoker::ExecuteCommandRequest(){
	// コマンドを積まれてなければスキップ
	if(commandRequests_.commandQueue_.empty()){
		return;
	}

	// 積まれたコマンドを全て実行する
	for(auto& cmd : commandRequests_.commandQueue_){
		cmd->Execute();
	}

	// 履歴に保存
	// currentCommandItr_ より先が残っていたら破棄（Redoのやり直し不可）
	commandHistory_.erase(std::next(currentCommandItr_),commandHistory_.end());

	// 新しい FrameCommand を履歴に push
	commandHistory_.push_back(std::make_unique<FrameCommand>(std::move(commandRequests_)));
	currentCommandItr_ = std::prev(commandHistory_.end());

	// 次のフレーム用に空の commandRequests_ を用意
	commandRequests_ = FrameCommand{};
}

void ObjectCommandInvoker::UndoCommand(){
	if(currentCommandItr_ == commandHistory_.end()){
		return;
	}

	// 逆順に Undo
	for(auto it = (*currentCommandItr_)->commandQueue_.rbegin();
		it != (*currentCommandItr_)->commandQueue_.rend(); ++it){
		(*it)->Undo();
	}

	if(currentCommandItr_ != commandHistory_.begin()){
		--currentCommandItr_;
	} else{
		currentCommandItr_ = commandHistory_.end(); // すべてUndo済み
	}
}

void ObjectCommandInvoker::RedoCommand(){
	if(commandHistory_.empty()){
		return;
	}

	if(currentCommandItr_ == commandHistory_.end()){
		currentCommandItr_ = commandHistory_.begin();
	} else if(std::next(currentCommandItr_) != commandHistory_.end()){
		++currentCommandItr_;
	} else{
		return; // これ以上Redoできない
	}

	// 順番通りに Redo
	for(auto& cmd : (*currentCommandItr_)->commandQueue_){
		cmd->Execute(); // Redoも基本的に再実行
	}
}