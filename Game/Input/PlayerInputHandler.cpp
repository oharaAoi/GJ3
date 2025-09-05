#include "PlayerInputHandler.h"

/// engine
#include "Engine/System/AUdio/AudioPlayer.h"

///command
#include "Game/Actor/Player/Command/PlayerMoveCommand.h"

/// invoker
#include "Game/Commands/ObjectCommandInvoker.h"
#include "Game/Manager/Collision/Common/MapCollisionSystem.h"

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

	// キー入力をチェック
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

	// ゲームパッド入力をチェック
	if(input->IsControllerConnected()){
		// 十字キー
		if(input->IsPressButton(kMoveLeftButton)){
			currentMoveDirection = MoveDirection::LEFT;
		}
		if(input->IsPressButton(kMoveRightButton)){
			currentMoveDirection = MoveDirection::RIGHT;
		}
		if(input->IsPressButton(kMoveUpButton)){
			currentMoveDirection = MoveDirection::UP;
		}
		if(input->IsPressButton(kMoveDownButton)){
			currentMoveDirection = MoveDirection::DOWN;
		}

		// 左スティック
		Vector2 leftStick = input->GetLeftJoyStick(0.5f);
		if(std::abs(leftStick.x) > std::abs(leftStick.y)){
			// 横移動
			if(leftStick.x < 0.f){
				currentMoveDirection = MoveDirection::LEFT;
			} else if(leftStick.x > 0.f){
				currentMoveDirection = MoveDirection::RIGHT;
			}
		} else if(std::abs(leftStick.x) < std::abs(leftStick.y)){
			// 縦移動
			if(leftStick.y < 0.f){
				currentMoveDirection = MoveDirection::DOWN;
			} else if(leftStick.y > 0.f){
				currentMoveDirection = MoveDirection::UP;
			}
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
			AudioPlayer::SinglShotPlay("panch.mp3", 0.3f);
		}
	} else if(preMoveDirection != currentMoveDirection){
		// 前回と違う方向
		isMoving = true;
		leftMoveEventTime_ = autoMoveStartDelay;
		AudioPlayer::SinglShotPlay("panch.mp3", 0.3f);
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