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
	// 範囲外か見る
	if (!OutOfRangeReference(index)) { return false; }

	// 進む方向1マス目ブロック
	IBlock* one = data[index.y][index.x].get();
	if (one == nullptr) {									// 何もないなら進める
		return true; 						
	} else if (one->GetType() == BlockType::Goal) {			// ゴールまでは進める
		return true;
	} else if (one->GetType() == BlockType::Wall) {			// 壁だから進めない
		return false;
	}

	// 進む方向2マス目ブロック
	index += direction;
	IBlock* two = data[index.y][index.x].get();
	// 1マス目が動かせるブロックで2マス目がないなら早期return
	if (one->GetType() == BlockType::NormalBlock || one->GetType() == BlockType::GhostBlock) {
		if (two == nullptr) {
			ChengeStage(direction, playerIndex);
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

bool MapCollisionSystem::OutOfRangeReference(const Vector2Int& index)
{
	const auto& data = stageRegistry_->GetStageData();
	size_t row = data.size();
	size_t col = data[0].size();
	// 範囲外では無いか検出
	if (index.x >= 0 && index.x < row && index.y >= 0 && index.y < col) { return true; }
	return false;
}

void MapCollisionSystem::ChengeStage(const Vector2Int& direction, const Vector2Int& playerIndex)
{
	// プレイヤーの元居たIndex番号からうめていく
	Vector2Int index = playerIndex;
	Vector2Int one = index + direction;
	Vector2Int two = one + direction;
	stageRegistry_->SetStageData(two, one);
}

bool MapCollisionSystem::CheckGhostBlock(const Vector2Int& index)
{
	const auto& data = stageRegistry_->GetStageData();
	// 範囲外では無いか検出
	if (!OutOfRangeReference(index)) { return false; }
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
