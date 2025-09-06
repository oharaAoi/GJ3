#include "ChangeBlockTypeGhostBlockToSpecial.h"

#include "Game/Manager/StageRegistry.h"
#include "Game/Actor/Block/BlockFactory.h"

ChangeBlockTypeGhostBlockToSpecial::ChangeBlockTypeGhostBlockToSpecial(StageRegistry* _stage,const Vector2Int& _index)
	:stage_(_stage),index_(_index){}
ChangeBlockTypeGhostBlockToSpecial::~ChangeBlockTypeGhostBlockToSpecial(){}

void ChangeBlockTypeGhostBlockToSpecial::Execute(){
	auto& stageData = stage_->GetStageDataRef();
	if(stageData[index_.y][index_.x] != nullptr){
		stageData[index_.y][index_.x]->Destroy();
	}
	stageData[index_.y][index_.x] = nullptr;
	stageData[index_.y][index_.x] = CreateBlock(static_cast<uint32_t>(BlockType::SpecialBlock));
	IBlock* newBlock = stageData[index_.y][index_.x].get();

	newBlock->Init(Engine::GetCanvas2d());
	newBlock->SetIndex(index_);

	// 位置を決定
	newBlock->SetPosition(stage_->CalculateTilePos(index_.y,index_.x));
	const Vector2& tileSize = stage_->GetTileSize();
	newBlock->GetSprite()->ReSetTextureSize(tileSize);
	newBlock->SetOffset(stage_->GetMapOffset());
	newBlock->SetTileSize(tileSize);
	// 元は SpecialBlockだったことを記憶しておく
	newBlock->SetIsSpecialBlock(true);
}

void ChangeBlockTypeGhostBlockToSpecial::Undo(){
	auto& stageData = stage_->GetStageDataRef();
	if(stageData[index_.y][index_.x] != nullptr){
		stageData[index_.y][index_.x]->Destroy();
	}
	stageData[index_.y][index_.x] = nullptr;
	stageData[index_.y][index_.x] = CreateBlock(static_cast<uint32_t>(BlockType::GhostBlock));
	IBlock* newBlock = stageData[index_.y][index_.x].get();

	newBlock->Init(Engine::GetCanvas2d());
	newBlock->SetIndex(index_);

	// 位置を決定
	newBlock->SetPosition(stage_->CalculateTilePos(index_.y,index_.x));
	const Vector2& tileSize = stage_->GetTileSize();
	newBlock->GetSprite()->ReSetTextureSize(tileSize);
	newBlock->SetOffset(stage_->GetMapOffset());
	newBlock->SetTileSize(tileSize);
	// 元は SpecialBlockだったことを記憶しておく
	newBlock->SetIsSpecialBlock(true);
}
