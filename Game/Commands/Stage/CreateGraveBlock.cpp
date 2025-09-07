#include "CreateGraveBlock.h"

/// stage
#include "Game/Manager/StageRegistry.h"

CreateGraveBlock::CreateGraveBlock(StageRegistry* _stageRegistry,Vector2Int _index)
	:stageRegistry_(_stageRegistry),index_(_index){}

CreateGraveBlock::~CreateGraveBlock(){}

void CreateGraveBlock::Execute(){
	if(isExecute_){
		return;
	}

	isSpecialBlock_ = false;
	if(stageRegistry_->GetStageData()[index_.y][index_.x] != nullptr){
		isSpecialBlock_ = stageRegistry_->GetStageData()[index_.y][index_.x]->GetIsSpecialBlock();
	}
	stageRegistry_->CreateStageData(index_,BlockType::GraveBlock);
	stageRegistry_->GetStageData()[index_.y][index_.x]->SetIsSpecialBlock(isSpecialBlock_);

	isExecute_ = true;
}

void CreateGraveBlock::Undo(){
	auto& block = stageRegistry_->GetStageData()[index_.y][index_.x];
	if(block && block->GetType() == BlockType::GraveBlock){
		stageRegistry_->ClearStageData(index_);
	}

	isExecute_ = false;
}
