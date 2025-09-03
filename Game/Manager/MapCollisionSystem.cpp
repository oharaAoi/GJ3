#include "MapCollisionSystem.h"

void MapCollisionSystem::Init(StageRegistry* stageRegistry)
{
	stageRegistry_ = stageRegistry;
}

void MapCollisionSystem::Update()
{
	UpdateSpanGhost();
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
	} else if (one->GetType() == BlockType::Goal ||			// ゴールまでは進める
		one->GetInGhost()) {
		return true;
	} else if (one->GetType() == BlockType::Wall) {			// 壁だから進めない
		return false;
	}

	// 進む方向2マス目ブロック
	index += direction;
	IBlock* two = data[index.y][index.x].get();
	// 1マス目が動かせるブロックで2マス目がないなら早期return
	if (one->GetType() == BlockType::NormalBlock || one->GetType() == BlockType::GhostBlock) {
		if (two == nullptr || two->GetInGhost()) {
			ChengeStage(direction, playerIndex);
			return true;
		}
	}

	return false;
}

void MapCollisionSystem::UpdateSpanGhost()
{
	if (!ghostUpdate_) { return; }

	// ステージデータからゴーストブロックを取得
	const auto& data = stageRegistry_->GetStageData();
	pairIndex_.clear();
	for (const auto& row : data) {
		for (const auto& col : row) {
			if (col == nullptr) { continue; }
			if (col->GetType() == BlockType::Ghost) { 
				stageRegistry_->ClearStageData(col->GetIndex()); 
				continue;
			}
			// ゴーストブロックじゃ無ければコンティニュー
			if (col->GetType() != BlockType::GhostBlock) { continue; }
			// ゴーストブロックだったら
			Vector2Int index = col->GetIndex();
			std::list<Vector2Int> indexs;
			indexs.push_back(Vector2Int{ 1,0 });
			indexs.push_back(Vector2Int{ -1,0 });
			indexs.push_back(Vector2Int{ 0,-1 });
			indexs.push_back(Vector2Int{ 0,1 });
			for (auto& _index : indexs) {
				if (CheckGhostBlock(index,_index)) {
					// ゴーストの出来る座標の間を取得する
					Vector2Int two = _index + _index + index;
					Vector2Int ghostIndex = {
						index.x * static_cast<int>(row.size()) + index.y,
						two.x * static_cast<int>(row.size()) + two.y,
					};
					bool hitPair = false;
					for (auto& pair : pairIndex_) {
						if ((pair.x == ghostIndex.x && pair.y == ghostIndex.y) ||
							(pair.y == ghostIndex.x && pair.x == ghostIndex.y)) {
							hitPair = true;
							continue;
						}
					}
					if (hitPair) { continue; }
					pairIndex_.push_back(ghostIndex);
				}
			}
		}
	}
	for (auto& pair : pairIndex_) {
		Vector2Int ghostIndex = SearchGhostIndex(pair);
		if (data[ghostIndex.y][ghostIndex.x] == nullptr || 
		   (data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::NormalBlock &&
			data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::GhostBlock)) {
			stageRegistry_->CreateStageData(ghostIndex, BlockType::Ghost);
		}
	}
	ghostUpdate_ = false;
}

bool MapCollisionSystem::OutOfRangeReference(const Vector2Int& index)
{
	const auto& data = stageRegistry_->GetStageData();
	size_t row = data.size();
	size_t col = data[0].size();
	// 範囲外では無いか検出
	if (index.y >= 0 && index.y < row && index.x >= 0 && index.x < col) { return true; }
	return false;
}

void MapCollisionSystem::ChengeStage(const Vector2Int& direction, const Vector2Int& playerIndex)
{
	// プレイヤーの元居たIndex番号からうめていく
	Vector2Int index = playerIndex;
	Vector2Int one = index + direction;
	Vector2Int two = one + direction;
	stageRegistry_->ChangeStageData(two, one);
	ghostUpdate_ = true;
}

bool MapCollisionSystem::CheckGhostBlock(const Vector2Int& playerIndex, const Vector2Int& index)
{
	const auto& data = stageRegistry_->GetStageData();
	// 1マス目の検知
	Vector2Int one = playerIndex + index;
	if (!OutOfRangeReference(one)) { return false; }
	// 何も無いか判定する
	if (data[one.y][one.x] != nullptr) { return false; }
	// 2マス目の検知
	// 範囲外では無いか検出
	Vector2Int two = one + index;
	if (!OutOfRangeReference(two)) { return false; }
	if (data[two.y][two.x] == nullptr) { return false; }
	// ゴーストブロックか判定
	if (data[two.y][two.x]->GetType() == BlockType::GhostBlock) {
		return true;
	}
	return false;
}

Vector2Int MapCollisionSystem::SearchGhostIndex(const Vector2Int& index)
{
	const auto& data = stageRegistry_->GetStageData();
	int col = static_cast<int>(data[0].size());
	return Vector2Int{
		((index.x + index.y) / 2) / col,
		((index.x + index.y) / 2) % col
	};
}
