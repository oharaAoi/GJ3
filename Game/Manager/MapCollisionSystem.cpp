#include "MapCollisionSystem.h"

void MapCollisionSystem::Init(StageRegistry* stageRegistry)
{
	stageRegistry_ = stageRegistry;

}

bool MapCollisionSystem::IsMovable(const Vector2Int& direction, const Vector2Int& playerIndex)
{
	// プレイヤーと進む方向のblockTypeとindexを取得
	const auto& data = stageRegistry_->GetStageData();

	// プレイヤーブロック
	Vector2Int index = playerIndex + direction;

	// 進む方向1マス目ブロック
	IBlock* one = data[index.x][index.y].get();
	if (one->GetType() == BlockType::Wall) {			// 壁だから進めない
		return false;								
	} else if (one->GetType() == BlockType::None) {		// 何もないなら進める
		ChengeStage(direction, playerIndex);
		return true; 
	}

	// 進む方向2マス目ブロック
	index += direction;
	IBlock* two = data[index.x][index.y].get();
	// 1マス目が動かせるブロックで2マス目がないなら早期return
	if (one->GetType() == BlockType::NormalBlock || one->GetType() == BlockType::GhostBlock) {
		if (two->GetType() == BlockType::None) {
			ChengeStage(direction, playerIndex, true);
			return true;
		}
	}

	return false;
}

void MapCollisionSystem::UpdateSpanGhost()
{
	// ステージデータからゴーストブロックを取得
	const auto& data = stageRegistry_->GetStageData();
	pairIndex_.clear();
	for (const auto& row : data) {
		for (const auto& col : row) {
			// ゴーストブロックじゃ無ければコンティニュー
			if (col->GetType() != BlockType::GhostBlock) { continue; }
			// ゴーストブロックだったら
			Vector2Int index = col->GetIndex();
			std::list<Vector2Int> indexs;
			indexs.push_back(index + Vector2Int{ 2,0 });
			indexs.push_back(index + Vector2Int{ -2,0 });
			indexs.push_back(index + Vector2Int{ 0,-2 });
			indexs.push_back(index + Vector2Int{ 0,2 });
			for (auto& _index : indexs) {
				if (CheckGhostBlock(_index)) {
					pairIndex_.push_back(
						Vector2Int{
							index.x * static_cast<int>(row.size()) + index.y,
							_index.x * static_cast<int>(row.size()) + _index.y,
						}
					);
				}
			}
		}
	}

}

void MapCollisionSystem::ChengeStage(const Vector2Int& direction, const Vector2Int& playerIndex, bool isSetBlock)
{
	// プレイヤーの元居たIndex番号からうめていく
	Vector2Int index = playerIndex;
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

bool MapCollisionSystem::CheckGhostBlock(const Vector2Int& index)
{
	const auto& data = stageRegistry_->GetStageData();
	size_t row = data.size();
	size_t col = data[0].size();
	// 範囲外では無いか検出
	if (index.x >= row || index.y >= col) { return false; }
	// ゴーストブロックか判定
	if (data[index.x][index.y]->GetType() == BlockType::GhostBlock) { 
		for (auto& pair : pairIndex_) {
			if ((pair.x == index.x && pair.y == index.y) ||
				(pair.y == index.x && pair.x == index.y)) {
				return false;
			}
		}
		return true;
	}
	return false;
}
