#include "PlayerGetGhostCommand.h"

#include "Game/Manager/Collision/Common/MapCollisionSystem.h"
#include "Game/Manager/StageRegistry.h"
#include "Game/Manager/GhostTakenEffectManager.h"

PlayerGetGhostCommand::PlayerGetGhostCommand(MapCollisionSystem* _mapCollision,const Vector2Int& _index)
	:mapCollision_(_mapCollision),index_(_index){}
PlayerGetGhostCommand::~PlayerGetGhostCommand(){}

void PlayerGetGhostCommand::Execute(){
	// おばけの数を増やす
	mapCollision_->AddGhostCounter();
	// 出来たおばけを判定する
	mapCollision_->GetSpecialBlockCollision()->RecursionBlockChecker(index_);
	// おばけを取得した際に出すおばけ吸収エフェクト
	Vector2 createPos = mapCollision_->GetStageRegi()->CalculateTilePos(index_.y, index_.x);
	GhostTakenEffectManager::GetInstance()->Create(createPos, mapCollision_->GetStageRegi()->GetTileSize() * 3.0f);
}
void PlayerGetGhostCommand::Undo(){
	// おばけの数を減らす
	mapCollision_->SubGhostCounter();
	// お化けを復活させる
	mapCollision_->GetStageRegi()->CreateStageData(index_,BlockType::Ghost);
	// 前フレームの配列におばけのindexを追加(Effect生成のため, 前の状態ではおばけがあったことを示す)
	mapCollision_->AddPreGhostThereIndies(index_);
	// 出来たおばけを判定する
	mapCollision_->GetSpecialBlockCollision()->RecursionBlockChecker(index_);

}