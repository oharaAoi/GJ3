#include "MapCollisionSystem.h"

constexpr Vector2Int VECTOR_LEFT = { -1,0 };
constexpr Vector2Int VECTOR_RIGHT = { 1,0 };
constexpr Vector2Int VECTOR_UP = { 0,-1 };
constexpr Vector2Int VECTOR_DOWN = { 0,1 };

constexpr Vector2Int VECTOR_LEFT_UP = { -1,-1 };
constexpr Vector2Int VECTOR_LEFT_DOWN = { -1,1 };

constexpr Vector2Int VECTOR_RIGHT_UP = { 1,-1 };
constexpr Vector2Int VECTOR_RIGHT_DOWN = { 1,1 };

void MapCollisionSystem::Init(StageRegistry* stageRegistry)
{
	stageRegistry_ = stageRegistry;

	indexs_[0] = (VECTOR_RIGHT);
	indexs_[1] = (VECTOR_LEFT);
	indexs_[2] = (VECTOR_UP);
	indexs_[3] = (VECTOR_DOWN);
	indexs_[4] = (VECTOR_LEFT_UP);
	indexs_[5] = (VECTOR_LEFT_DOWN);
	indexs_[6] = (VECTOR_RIGHT_UP);
	indexs_[7] = (VECTOR_RIGHT_DOWN);

	// 初期化時に一回だけ更新をかけてゴーストを反映させる
	playerIndex_ = stageRegistry_->GetStartIndex();
	ghostUpdate_ = true;
	UpdateSpanGhost();
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
		one->GetType() == BlockType::Ghost) {
		ChangeGrave(index);
		return true;
	} else if (one->GetType() == BlockType::Wall ||			// 壁だから進めない
		one->GetType() == BlockType::GraveBlock) {			
		return false;
	}

	// 進む方向2マス目ブロック
	index += direction;
	IBlock* two = data[index.y][index.x].get();
	// 1マス目が動かせるブロックで2マス目がないなら早期return
	if (one->GetType() == BlockType::NormalBlock || 
		one->GetType() == BlockType::GhostBlock ||
		one->GetType() == BlockType::SpecialBlock) {
		if (two == nullptr || two->GetType() == BlockType::Ghost) {
			playerIndex_ = playerIndex + direction;;
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
			// 特殊ブロックは判定しない
			if (col == nullptr) { continue; }
			else { col->SetIsChengeBlock(false); }
			if (col->GetIsSpecialBlock()) { continue; }
			// おばけをリセットする
			if (col->GetType() == BlockType::Ghost) { 
				stageRegistry_->ClearStageData(col->GetIndex()); 
				continue;
			}
			// ゴーストブロックじゃ無ければコンティニュー
			if (col->GetType() != BlockType::GhostBlock) { continue; }
			// ゴーストブロックだったら
			Vector2Int index = col->GetIndex();
			CheckTokenGhost(index);
		}
	}
	// 実際のおばけ生成ポイント
	CreateTokenGhost();
	ghostUpdate_ = false;

	// 特殊ブロックの入り判定のゴーストを作成する処理
	pairIndex_.clear();
	for (const auto& row : data) {
		for (const auto& col : row) {
			if (col == nullptr) { continue; }
			RecursionSpacialBlockChecker(col->GetIndex());
		}
	}
	// 特殊ブロックの出判定を取る
	ChangeSpecialBlock();

	// 全ゴーストの生成をする
	pairIndex_.clear();
	for (const auto& row : data) {
		for (const auto& col : row) {
			// nullは判定しない
			if (col == nullptr) { continue; }
			// おばけをリセットする
			if (col->GetType() == BlockType::Ghost) {
				stageRegistry_->ClearStageData(col->GetIndex());
				continue;
			}
			// ゴーストブロックじゃ無ければコンティニュー
			if (col->GetType() != BlockType::GhostBlock) { continue; }
			// ゴーストブロックだったら
			Vector2Int index = col->GetIndex();
			CheckTokenGhost(index);
		}
	}
	// 実際のおばけ生成ポイント
	CreateTokenGhost();
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

void MapCollisionSystem::CheckTokenGhost(const Vector2Int& index)
{
	const auto& data = stageRegistry_->GetStageData();
	const size_t row_size = data[0].size();
	// ゴーストができるか判定する
	// できるペアをpairIndexに保存する
	for (size_t i = 0; i < (indexs_.size() / 2);++i) {
		if (CheckGhostBlock(index, indexs_[i])) {
			// ゴーストの出来る座標の間を取得する
			Vector2Int two = indexs_[i] + indexs_[i] + index;
			Vector2Int ghostIndex = {
				index.x * static_cast<int>(row_size) + index.y,
				two.x * static_cast<int>(row_size) + two.y,
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
	if(ghostUpdate_ && data[two.y][two.x]->GetIsSpecialBlock()) { return false; }
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

void MapCollisionSystem::CreateTokenGhost()
{
	// 実際のおばけ生成ポイント
	const auto& data = stageRegistry_->GetStageData();
	for (auto& pair : pairIndex_) {
		Vector2Int ghostIndex = SearchGhostIndex(pair);
		if (data[ghostIndex.y][ghostIndex.x] == nullptr ||
			(data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::NormalBlock &&
				data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::GhostBlock &&
				data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::SpecialBlock)) {
			// ブロックを押したタイミングでゴーストに当たっているなら
			if (ghostIndex.x == playerIndex_.x && ghostIndex.y == playerIndex_.y) {
				++ghostCounter_;
				stageRegistry_->CreateStageData(ghostIndex, BlockType::GraveBlock);
			} else {
				stageRegistry_->CreateStageData(ghostIndex, BlockType::Ghost);
			}
		}
	}
}

void MapCollisionSystem::ChangeGrave(const Vector2Int& index)
{
	const auto& data = stageRegistry_->GetStageData();
	if (data[index.y][index.x] == nullptr) { return; }
	if (data[index.y][index.x]->GetType() == BlockType::Ghost) {
		++ghostCounter_;
		stageRegistry_->CreateStageData(index, BlockType::GraveBlock);
		ChangeSpecialBlock();
	}
}

void MapCollisionSystem::ChangeSpecialBlock()
{
	// ステージデータから特殊ブロックを取得
	const auto& data = stageRegistry_->GetStageData();

	for (const auto& row : data) {
		for (const auto& col : row) {
			// 特殊ブロック以外ならコンティニュー
			if (col == nullptr || !col->GetIsSpecialBlock()) { continue; }
			// 特殊ブロックなら8方向に判定を取る
			Vector2Int sIndex = col->GetIndex();
			bool isHit = false;
			// おばけを探索する
			for (auto& index : indexs_) {
				Vector2Int nextIndex = index + sIndex;
				if (data[nextIndex.y][nextIndex.x] == nullptr ||
					data[nextIndex.y][nextIndex.x]->GetType() != BlockType::Ghost) { continue; }
				isHit = true;
				break;
			}
			// 判定結果を反映する
			if (!isHit) {
				if (data[sIndex.y][sIndex.x]->GetIsSpecialBlock()) {
					stageRegistry_->CreateStageData(sIndex, BlockType::SpecialBlock);
				}
			}
		}
	}
}

void MapCollisionSystem::RecursionSpacialBlockChecker(const Vector2Int& _index)
{
	// ステージデータからおばけを取得
	const auto& data = stageRegistry_->GetStageData();
	// おばけ以外ならreturn
	if (data[_index.y][_index.x] == nullptr ||
		data[_index.y][_index.x]->GetType() != BlockType::Ghost) { return; }
	// おばけだったら中心から3x3に特殊ブロックを判定する
	for (auto& index : indexs_) {
		Vector2Int nextIndex = index + _index;
		// 特殊ブロック以外ならcontinue
		if (data[nextIndex.y][nextIndex.x] == nullptr ||
			!data[nextIndex.y][nextIndex.x]->GetIsSpecialBlock()) { continue; }
		if (data[nextIndex.y][nextIndex.x]->GetIsChengeBlock()) { continue; }
		// 特殊ブロックだったら
		// ゴーストブロックに変換する
		if (data[nextIndex.y][nextIndex.x]->GetType() == BlockType::SpecialBlock) {
			stageRegistry_->CreateStageData(nextIndex, BlockType::GhostBlock);
		}
		data[nextIndex.y][nextIndex.x]->SetIsChengeBlock(true);
		// ゴーストブロックにしておばけが出来ないか判定する
		CheckTokenGhost(nextIndex);
		// おばけが出来ているなら作成する
		for (auto& pair : pairIndex_) {
			Vector2Int ghostIndex = SearchGhostIndex(pair);
			if (data[ghostIndex.y][ghostIndex.x] == nullptr ||
				(data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::NormalBlock &&
					data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::GhostBlock &&
					data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::SpecialBlock)) {
				// ブロックを押したタイミングでゴーストに当たっているなら
				if (ghostIndex.x == playerIndex_.x && ghostIndex.y == playerIndex_.y) {
					++ghostCounter_;
					stageRegistry_->CreateStageData(ghostIndex, BlockType::GraveBlock);
				} else {
					stageRegistry_->CreateStageData(ghostIndex, BlockType::Ghost);
					RecursionSpacialBlockChecker(ghostIndex);
				}
			}
		}
	}
}
