#include "PlayerGetGhostCommand.h"

#include "Game/Manager/Collision/Common/MapCollisionSystem.h"
#include "Game/Manager/StageRegistry.h"

PlayerGetGhostCommand::PlayerGetGhostCommand(MapCollisionSystem* _mapCollision,const Vector2Int& _index)
	:mapCollision_(_mapCollision),index_(_index){}
PlayerGetGhostCommand::~PlayerGetGhostCommand(){}

void PlayerGetGhostCommand::Execute(){
	// おばけの数を増やす
	mapCollision_->AddGhostCount();
}
void PlayerGetGhostCommand::Undo(){
	// おばけの数を減らす
	mapCollision_->SubGhostCounter();
	// お化けを復活させる
	mapCollision_->GetStageRegi()->CreateStageData(index_,BlockType::Ghost);

}