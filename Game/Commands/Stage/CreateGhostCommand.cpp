#include "CreateGhostCommand.h"

CreateGhostCommand::CreateGhostCommand(StageRegistry* _stageRegistry,const Vector2Int& _index)
	:stageRegistry_(_stageRegistry),index_(_index){}
CreateGhostCommand::~CreateGhostCommand(){}

void CreateGhostCommand::Execute(){
	if(isExecute_){ return; }
	isExecute_ = true;
	stageRegistry_->CreateStageData(index_,BlockType::Ghost);
}

void CreateGhostCommand::Undo(){
	stageRegistry_->ClearStageData(index_);
}
