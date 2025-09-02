#include "MapCollisionSystem.h"

void MapCollisionSystem::Init(StageRegistry* stageRegistry)
{
	stageRegistry_ = stageRegistry;

}

bool MapCollisionSystem::IsMovable(const Vector2Int& direction, IBlock* player)
{
	// プレイヤーと進む方向のblockTypeとindexを取得
	const auto& data = stageRegistry_->GetStageData();

	// プレイヤーブロック
	IBlock* p = player;
	Vector2Int index = p->GetIndex() + direction;

	// 進む方向1マス目ブロック
	IBlock* one = data[index.x][index.y].get();
	if (one->GetType() == BlockType::Wall) {			// 壁だから進めない
		return false;								
	} else if (one->GetType() == BlockType::None) {		// 何もないなら進める
		ChengeStage(direction, player);
		return true; 
	}

	// 進む方向2マス目ブロック
	index += direction;
	IBlock* two = data[index.x][index.y].get();
	// 1マス目が動かせるブロックで2マス目がないなら早期return
	if (one->GetType() == BlockType::NormalBlock || one->GetType() == BlockType::GhostBlock) {
		if (two->GetType() == BlockType::None) {
			ChengeStage(direction, player, true);
			return true;
		}
	}

	return false;
}

void MapCollisionSystem::ChengeStage(const Vector2Int& direction, IBlock* player, bool isSetBlock)
{
	// プレイヤーの元居たIndex番号からうめていく
	Vector2Int index = player->GetIndex();
	const auto& data = stageRegistry_->GetStageData();

	data[index.x][index.y]->SetType(BlockType::None);
	index += direction;
	BlockType type = data[index.x][index.y]->GetType();
	data[index.x][index.y]->SetType(BlockType::Player);
	if (isSetBlock) {
		index += direction;
		data[index.x][index.y]->SetType(type);
	}
}