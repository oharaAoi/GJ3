#include "MoveBlockCommand.h"

/// stage
#include "Game/Manager/StageRegistry.h"
#include "Game/Manager/Collision/Common/MapCollisionSystem.h"

MoveBlockCommand::MoveBlockCommand(StageRegistry* _stageRegistry,MapCollisionSystem* _mapCollisionSystem,const Vector2Int& _fromIndex,const Vector2Int& _toIndex)
	:stageRegistry_(_stageRegistry),mapCollisionSystem_(_mapCollisionSystem),fromIndex_(_fromIndex),toIndex_(_toIndex){}
MoveBlockCommand::~MoveBlockCommand(){}

void MoveBlockCommand::Execute(){
	if(isExecute_){
		return;
	}
	auto& stageData_ = stageRegistry_->GetStageDataRef();

	if(stageData_[toIndex_.y][toIndex_.x] != nullptr){
		stageData_[toIndex_.y][toIndex_.x]->Destroy();
	}
	stageData_[toIndex_.y][toIndex_.x] = std::move(stageData_[fromIndex_.y][fromIndex_.x]);
	stageData_[toIndex_.y][toIndex_.x]->SetIndex(toIndex_);
	stageData_[fromIndex_.y][fromIndex_.x] = nullptr;

	mapCollisionSystem_->GetGhostBlockCollision()->SetGhostUpdate(true);

	isExecute_ = true;
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

	mapCollisionSystem_->GetGhostBlockCollision()->SetGhostUpdate(true);

	isExecute_ = false;
}