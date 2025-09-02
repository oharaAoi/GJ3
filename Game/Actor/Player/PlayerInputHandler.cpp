#include "PlayerInputHandler.h"

///command
#include "Command/PlayerMoveCommand.h"

/// invoker
#include "Game/Commands/ObjectCommandInvoker.h"

PlayerInputHandler::PlayerInputHandler(){}
PlayerInputHandler::~PlayerInputHandler(){}

void PlayerInputHandler::HandleInput(){
	if(!player_){
		return;
	}

	// 移動方向を決定
	Vector2Int moveDirection = DecideMoveDirection();

	if(moveDirection.x == 0 && moveDirection.y == 0){
		return;
	}

	// 移動コマンドを生成
	std::unique_ptr<IPlayerCommand> command = nullptr;
	if(moveDirection.x == 1){
		command = std::make_unique<PlayerRightMoveCommand>(player_);
	} else if(moveDirection.x == -1){
		command = std::make_unique<PlayerLeftMoveCommand>(player_);
	} else if(moveDirection.y == 1){
		command = std::make_unique<PlayerDownMoveCommand>(player_);
	} else if(moveDirection.y == -1){
		command = std::make_unique<PlayerUpMoveCommand>(player_);
	}
	if(!command){
		return;
	}
	// コマンドを invoker にセット
	ObjectCommandInvoker::GetInstance().PushCommand(std::move(command));
}

Vector2Int PlayerInputHandler::DecideMoveDirection(){
	Input* input = Input::GetInstance();

	for(auto moveLeftKey : kMoveLeftKey){
		if(input->IsTriggerKey(moveLeftKey)){
			return Vector2Int(-1,0);
		}
	}
	for(auto moveRightKey : kMoveRightKey){
		if(input->IsTriggerKey(moveRightKey)){
			return Vector2Int(1,0);
		}
	}
	for(auto moveUpKey : kMoveUpKey){
		if(input->IsTriggerKey(moveUpKey)){
			return Vector2Int(0,-1);
		}
	}
	for(auto moveDownKey : kMoveDownKey){
		if(input->IsTriggerKey(moveDownKey)){
			return Vector2Int(0,1);
		}
	}
	return Vector2Int(0,0);
}

