#include "PlayerInputHandler.h"

/// engine
#include "Engine/System/AUdio/AudioPlayer.h"

///command
#include "Game/Commands/Player/PlayerMoveCommand.h"

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

	AudioPlayer::SinglShotPlay("panch.mp3",0.3f);

	// animationする
	player_->SetIsAnimation(true);

	// 移動コマンドを生成
	std::unique_ptr<IPlayerCommand> command = nullptr;
	if(moveDirection.x == 1){
		command = std::make_unique<PlayerRightMoveCommand>(player_,player_->GetIndex());
	} else if(moveDirection.x == -1){
		command = std::make_unique<PlayerLeftMoveCommand>(player_,player_->GetIndex());
	} else if(moveDirection.y == 1){
		command = std::make_unique<PlayerDownMoveCommand>(player_,player_->GetIndex());
	} else if(moveDirection.y == -1){
		command = std::make_unique<PlayerUpMoveCommand>(player_,player_->GetIndex());
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

	isKeyInput_ = false;
	isPadInput_ = false;

	// キー入力をチェック
	for(auto moveLeftKey : kMoveLeftKey){
		if(input->IsPressKey(moveLeftKey)){
			currentMoveDirection = MoveDirection::LEFT;
			isKeyInput_ = true;
		}
	}
	for(auto moveRightKey : kMoveRightKey){
		if(input->IsPressKey(moveRightKey)){
			currentMoveDirection = MoveDirection::RIGHT;
			isKeyInput_ = true;
		}
	}
	for(auto moveUpKey : kMoveUpKey){
		if(input->IsPressKey(moveUpKey)){
			currentMoveDirection = MoveDirection::UP;
			isKeyInput_ = true;
		}
	}
	for(auto moveDownKey : kMoveDownKey){
		if(input->IsPressKey(moveDownKey)){
			currentMoveDirection = MoveDirection::DOWN;
			isKeyInput_ = true;
		}
	}

	// ゲームパッド入力をチェック
	if(input->IsControllerConnected()){
		// 十字キー
		if(input->IsPressButton(kMoveLeftButton)){
			currentMoveDirection = MoveDirection::LEFT;
			isKeyInput_ = true;
		}
		if(input->IsPressButton(kMoveRightButton)){
			currentMoveDirection = MoveDirection::RIGHT;
			isKeyInput_ = true;
		}
		if(input->IsPressButton(kMoveUpButton)){
			currentMoveDirection = MoveDirection::UP;
			isKeyInput_ = true;
		}
		if(input->IsPressButton(kMoveDownButton)){
			currentMoveDirection = MoveDirection::DOWN;
			isKeyInput_ = true;
		}

		// 左スティック
		Vector2 leftStick = input->GetLeftJoyStick(0.4f);
		if(std::abs(leftStick.x) > std::abs(leftStick.y)){
			// 横移動
			if(leftStick.x < 0.f){
				currentMoveDirection = MoveDirection::LEFT;
				isKeyInput_ = true;
			} else if(leftStick.x > 0.f){
				currentMoveDirection = MoveDirection::RIGHT;
				isKeyInput_ = true;
			}
		} else if(std::abs(leftStick.x) < std::abs(leftStick.y)){
			// 縦移動
			if(leftStick.y < 0.f){
				currentMoveDirection = MoveDirection::DOWN;
				isKeyInput_ = true;
			} else if(leftStick.y > 0.f){
				currentMoveDirection = MoveDirection::UP;
				isKeyInput_ = true;
			}
		}
	}

	leftMoveEventTime_ -= GameTimer::DeltaTime();
	leftMoveEventTime_ = std::max(0.f,leftMoveEventTime_);

	if(currentMoveDirection == MoveDirection::NONE){
		preMoveDirection = MoveDirection::NONE;
		return Vector2Int(0,0);
	}

	bool isMoving = false;
	if(preMoveDirection == currentMoveDirection){
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