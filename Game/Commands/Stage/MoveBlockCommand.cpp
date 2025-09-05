#include "MoveBlockCommand.h"

/// stage
#include "Game/Manager/StageRegistry.h"
#include "Game/Manager/Collision/Map/GhostBlockCollision.h"

MoveBlockCommand::MoveBlockCommand(StageRegistry* _stageRegistry,GhostBlockCollision* _ghostBlockCollision,const Vector2Int& _fromIndex,const Vector2Int& _toIndex)
	:stageRegistry_(_stageRegistry),ghostBlockCollision_(_ghostBlockCollision),fromIndex_(_fromIndex),toIndex_(_toIndex){}
MoveBlockCommand::~MoveBlockCommand(){}

void MoveBlockCommand::Execute(){
	auto& stageData_ = stageRegistry_->GetStageDataRef();

	if(stageData_[toIndex_.y][toIndex_.x] != nullptr){
		stageData_[toIndex_.y][toIndex_.x]->Destroy();
	}
	stageData_[toIndex_.y][toIndex_.x] = std::move(stageData_[fromIndex_.y][fromIndex_.x]);
	stageData_[toIndex_.y][toIndex_.x]->SetIndex(toIndex_);
	stageData_[fromIndex_.y][fromIndex_.x] = nullptr;

	ghostBlockCollision_->SetGhostUpdate(true);

}
void MoveBlockCommand::Undo(){
	Vector2Int copyIndex = toIndex_;
	auto& stageData_ = stageRegistry_->GetStageDataRef();

	if(stageData_[fromIndex_.y][fromIndex_.x] != nullptr){
		stageData_[fromIndex_.y][fromIndex_.x]->Destroy();
	}
	stageData_[fromIndex_.y][fromIndex_.x] = std::move(stageData_[toIndex_.y][toIndex_.x]);
	stageData_[fromIndex_.y][fromIndex_.x]->SetIndex(fromIndex_);
	stageData_[toIndex_.y][toIndex_.x] = nullptr;

	ghostBlockCollision_->SetGhostUpdate(true);
}