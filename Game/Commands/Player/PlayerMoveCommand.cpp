#include "PlayerMoveCommand.h"

constexpr Vector2Int MOVE_VECTOR_LEFT = {-1,0};
constexpr Vector2Int MOVE_VECTOR_RIGHT = {1,0};
constexpr Vector2Int MOVE_VECTOR_UP = {0,-1};
constexpr Vector2Int MOVE_VECTOR_DOWN = {0,1};

void PlayerLeftMoveCommand::Execute(){
	host_->SetIndex(host_->GetIndex() + MOVE_VECTOR_LEFT);

	// 向きを左に変える
	preDirection_ = host_->GetSpriteDirection();
	host_->SetSpriteDirection(Player::SpriteDirection::LEFT);
}

void PlayerLeftMoveCommand::Undo(){
	host_->SetIndex(host_->GetIndex() - MOVE_VECTOR_LEFT);

	// 向きを元に戻す
	host_->SetSpriteDirection(preDirection_);
}

void PlayerRightMoveCommand::Execute(){
	host_->SetIndex(host_->GetIndex() + MOVE_VECTOR_RIGHT);

	// 向きを右に変える
	preDirection_ = host_->GetSpriteDirection();
	host_->SetSpriteDirection(Player::SpriteDirection::RIGHT);
}
void PlayerRightMoveCommand::Undo(){
	host_->SetIndex(host_->GetIndex() - MOVE_VECTOR_RIGHT);

	// 向きを元に戻す
	host_->SetSpriteDirection(preDirection_);
}

void PlayerUpMoveCommand::Execute(){
	host_->SetIndex(host_->GetIndex() + MOVE_VECTOR_UP);

	// 向きを左に変える
	preDirection_ = host_->GetSpriteDirection();
	host_->SetSpriteDirection(Player::SpriteDirection::LEFT);
}
void PlayerUpMoveCommand::Undo(){
	host_->SetIndex(host_->GetIndex() - MOVE_VECTOR_UP);

	// 向きを元に戻す
	host_->SetSpriteDirection(preDirection_);
}

void PlayerDownMoveCommand::Execute(){
	host_->SetIndex(host_->GetIndex() + MOVE_VECTOR_DOWN);

	// 向きを右に変える
	preDirection_ = host_->GetSpriteDirection();
	host_->SetSpriteDirection(Player::SpriteDirection::RIGHT);
}
void PlayerDownMoveCommand::Undo(){
	host_->SetIndex(host_->GetIndex() - MOVE_VECTOR_DOWN);

	// 向きを元に戻す
	host_->SetSpriteDirection(preDirection_);
}