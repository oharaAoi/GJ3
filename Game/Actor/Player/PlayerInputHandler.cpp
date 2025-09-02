#include "PlayerInputHandler.h"

///command
#include "Command/PlayerMoveCommand.h"

/// invoker
#include "Game/Commands/ObjectCommandInvoker.h"
#include "Game/Manager/MapCollisionSystem.h"

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
	if(!player_->GetMapCollision()->IsMovable(moveDirection,player_->GetIndex())){
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

	MoveDirection currentMoveDirection = MoveDirection::NONE;

	for(auto moveLeftKey : kMoveLeftKey){
		if(input->IsPressKey(moveLeftKey)){
			currentMoveDirection = MoveDirection::LEFT;
		}
	}
	for(auto moveRightKey : kMoveRightKey){
		if(input->IsPressKey(moveRightKey)){
			currentMoveDirection = MoveDirection::RIGHT;
		}
	}
	for(auto moveUpKey : kMoveUpKey){
		if(input->IsPressKey(moveUpKey)){
			currentMoveDirection = MoveDirection::UP;
		}
	}
	for(auto moveDownKey : kMoveDownKey){
		if(input->IsPressKey(moveDownKey)){
			currentMoveDirection = MoveDirection::DOWN;
		}
	}

	if(currentMoveDirection == MoveDirection::NONE){
		preMoveDirection = MoveDirection::NONE;
		leftMoveEventTime_ = 0.f;
		return Vector2Int(0,0);
	}

	bool isMoving = false;
	if(preMoveDirection == currentMoveDirection){
		leftMoveEventTime_ -= GameTimer::DeltaTime();
		if(leftMoveEventTime_ <= 0.f){
			isMoving = true;
			leftMoveEventTime_ = autoMoveStepInterval;
		}
	} else if(preMoveDirection != currentMoveDirection){
		// 前回と違う方向
		isMoving = true;
		leftMoveEventTime_ = autoMoveStartDelay;
	}

	preMoveDirection = currentMoveDirection;

	if(!isMoving){
		return Vector2Int(0,0);
	}

	switch(currentMoveDirection){
		case PlayerInputHandler::MoveDirection::LEFT:
			return Vector2Int(-1,0);
		case PlayerInputHandler::MoveDirection::RIGHT:
			return Vector2Int(1,0);
		case PlayerInputHandler::MoveDirection::UP:
			return Vector2Int(0,-1);
		case PlayerInputHandler::MoveDirection::DOWN:
			return Vector2Int(0,1);
		default:
			return Vector2Int(0,0);
			break;
	}


}