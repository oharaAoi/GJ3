#include "MoveBlockCommand.h"

/// stage
#include "Game/Manager/StageRegistry.h"

MoveBlockCommand::MoveBlockCommand(StageRegistry* _stageRegistry,const Vector2Int& _fromIndex,const Vector2Int& _toIndex)
	:stageRegistry_(_stageRegistry),fromIndex_(_fromIndex),toIndex_(_toIndex){}
MoveBlockCommand::~MoveBlockCommand(){}

void MoveBlockCommand::Execute(){
	stageRegistry_->ChangeStageData(toIndex_,fromIndex_);
}
void MoveBlockCommand::Undo(){
	stageRegistry_->ChangeStageData(fromIndex_,toIndex_);
}