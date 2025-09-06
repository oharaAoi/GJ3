#include "ChangeBlockTypeSpecialToGhostBlock.h"

#include "Game/Manager/StageRegistry.h"
#include "Game/Actor/Block/BlockFactory.h"

ChangeBlockTypeSpecialToGhostBlock::ChangeBlockTypeSpecialToGhostBlock(StageRegistry* _stageBlock,const Vector2Int& _index)
	:pStageRegistry_(_stageBlock),index_(_index){}
ChangeBlockTypeSpecialToGhostBlock::~ChangeBlockTypeSpecialToGhostBlock(){};

void ChangeBlockTypeSpecialToGhostBlock::Execute(){
	auto& stageData = pStageRegistry_->GetStageDataRef();
	if(stageData[index_.y][index_.x] != nullptr){
		stageData[index_.y][index_.x]->Destroy();
	}
	stageData[index_.y][index_.x] = nullptr;
	stageData[index_.y][index_.x] = CreateBlock(static_cast<uint32_t>(BlockType::GhostBlock));
	IBlock* newBlock = stageData[index_.y][index_.x].get();

	newBlock->Init(Engine::GetCanvas2d());
	newBlock->SetIndex(index_);

	// 位置を決定
	newBlock->SetPosition(pStageRegistry_->CalculateTilePos(index_.y,index_.x));
	const Vector2& tileSize = pStageRegistry_->GetTileSize();
	newBlock->GetSprite()->ReSetTextureSize(tileSize);
	newBlock->SetOffset(pStageRegistry_->GetMapOffset());
	newBlock->SetTileSize(tileSize);
	// 元は SpecialBlockだったことを記憶しておく
	newBlock->SetIsSpecialBlock(true);
}
void ChangeBlockTypeSpecialToGhostBlock::Undo(){
	auto& stageData = pStageRegistry_->GetStageDataRef();
	if(stageData[index_.y][index_.x] != nullptr){
		stageData[index_.y][index_.x]->Destroy();
	}
	stageData[index_.y][index_.x] = nullptr;
	stageData[index_.y][index_.x] = CreateBlock(static_cast<uint32_t>(BlockType::SpecialBlock));
	IBlock* newBlock = stageData[index_.y][index_.x].get();

	newBlock->Init(Engine::GetCanvas2d());
	newBlock->SetIndex(index_);

	// 位置を決定
	newBlock->SetPosition(pStageRegistry_->CalculateTilePos(index_.y,index_.x));
	const Vector2& tileSize = pStageRegistry_->GetTileSize();
	newBlock->GetSprite()->ReSetTextureSize(tileSize);
	newBlock->SetOffset(pStageRegistry_->GetMapOffset());
	newBlock->SetTileSize(tileSize);
	// 元は SpecialBlockだったことを記憶しておく
	newBlock->SetIsSpecialBlock(true);
}