#include "PlayerMoveCommand.h"

constexpr Vector2Int MOVE_VECTOR_LEFT = {-1,0};
constexpr Vector2Int MOVE_VECTOR_RIGHT = {1,0};
constexpr Vector2Int MOVE_VECTOR_UP = {0,-1};
constexpr Vector2Int MOVE_VECTOR_DOWN = {0,1};

void PlayerLeftMoveCommand::Execute(){
	host_->SetIndex(host_->GetIndex() + MOVE_VECTOR_LEFT);
}

void PlayerLeftMoveCommand::Undo(){
	host_->SetIndex(host_->GetIndex() - MOVE_VECTOR_LEFT);
}

void PlayerRightMoveCommand::Execute(){
	host_->SetIndex(host_->GetIndex() + MOVE_VECTOR_RIGHT);
}
void PlayerRightMoveCommand::Undo(){
	host_->SetIndex(host_->GetIndex() - MOVE_VECTOR_RIGHT);
}

void PlayerUpMoveCommand::Execute(){
	host_->SetIndex(host_->GetIndex() + MOVE_VECTOR_UP);
}
void PlayerUpMoveCommand::Undo(){
	host_->SetIndex(host_->GetIndex() - MOVE_VECTOR_UP);
}

void PlayerDownMoveCommand::Execute(){
	host_->SetIndex(host_->GetIndex() + MOVE_VECTOR_DOWN);
}
void PlayerDownMoveCommand::Undo(){
	host_->SetIndex(host_->GetIndex() - MOVE_VECTOR_DOWN);
}