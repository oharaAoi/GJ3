#include "SpecialBlockCollision.h"

#include "Game/Manager/Collision/Common/MapCollisionSystem.h"
#include "Game/Manager/StageRegistry.h"

/// command
#include "Game/Commands/ObjectCommandInvoker.h"
#include "Game/Commands/Stage/PlayerGetGhostCommand.h"
#include "Game/Commands/Stage/CreateGraveBlock.h"

void SpecialBlockCollision::Init(MapCollisionSystem* system)
{
	system_ = system;
}

void SpecialBlockCollision::ChangeBlock()
{
	// ステージデータから特殊ブロックを取得
	const auto& data = system_->GetStageRegi()->GetStageData();

	for (const auto& row : data) {
		for (const auto& col : row) {
			// 特殊ブロック以外ならコンティニュー
			if (col == nullptr || !col->GetIsSpecialBlock()) { continue; }
			// 特殊ブロックなら8方向に判定を取る
			Vector2Int sIndex = col->GetIndex();
			bool isHit = false;
			// おばけを探索する
			for (auto& index : system_->GetNeighborOffsets()) {
				Vector2Int nextIndex = index + sIndex;
				if (data[nextIndex.y][nextIndex.x] == nullptr ||
					data[nextIndex.y][nextIndex.x]->GetType() != BlockType::Ghost) {
					continue;
				}
				isHit = true;
				break;
			}
			// 判定結果を反映する
			if (!isHit) {
				if (data[sIndex.y][sIndex.x]->GetIsSpecialBlock()) {
					system_->GetStageRegi()->CreateStageData(sIndex, BlockType::SpecialBlock);
				}
			}
		}
	}
}

void SpecialBlockCollision::RecursionBlockChecker(const Vector2Int& _index)
{
	// ステージデータからおばけを取得
	const auto& data = system_->GetStageRegi()->GetStageData();
	// おばけ以外ならreturn
	if (data[_index.y][_index.x]->GetType() != BlockType::Ghost) { return; }
	// おばけだったら中心から3x3に特殊ブロックを判定する
	for (auto& index : system_->GetNeighborOffsets()) {
		Vector2Int nextIndex = index + _index;
		// 特殊ブロック以外ならcontinue
		if (data[nextIndex.y][nextIndex.x] == nullptr ||
			!data[nextIndex.y][nextIndex.x]->GetIsSpecialBlock()) {
			continue;
		}
		if (data[nextIndex.y][nextIndex.x]->GetIsChengeBlock()) { continue; }
		// 特殊ブロックだったら
		// ゴーストブロックに変換する
		if (data[nextIndex.y][nextIndex.x]->GetType() == BlockType::SpecialBlock) {
			system_->GetStageRegi()->CreateStageData(nextIndex, BlockType::GhostBlock);
		}
		data[nextIndex.y][nextIndex.x]->SetIsChengeBlock(true);
		// ゴーストブロックにしておばけが出来ないか判定する
		system_->CheckTokenGhost(nextIndex);
		// おばけが出来ているなら作成する
		for (auto& pair : system_->GetPairIndex()) {
			Vector2Int ghostIndex = system_->SearchGhostIndex(pair);
			if (!system_->OutOfRangeReference(ghostIndex)) { continue; }
			if (data[ghostIndex.y][ghostIndex.x] == nullptr ||
				(data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::NormalBlock &&
					data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::GhostBlock &&
					data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::SpecialBlock &&
					data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::LimitBlock &&
					data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::Ghost)) {
				// ブロックを押したタイミングでゴーストに当たっているなら
				if (system_->CheckLimitBlock(ghostIndex)) {
					const auto& playerIndex = system_->GetPlayerIndex();
					if (ghostIndex.x == playerIndex.x && ghostIndex.y == playerIndex.y) {
						auto PlayerGetGhostCommandCommand = std::make_unique<PlayerGetGhostCommand>(system_,playerIndex);
						ObjectCommandInvoker::GetInstance().PushCommand(std::move(PlayerGetGhostCommandCommand));

						auto createGraveCommand = std::make_unique<CreateGraveBlock>(system_->GetStageRegi(),ghostIndex);
						ObjectCommandInvoker::GetInstance().PushCommand(std::move(createGraveCommand));
					} else {
						system_->GetStageRegi()->CreateStageData(ghostIndex, BlockType::Ghost);
						RecursionBlockChecker(ghostIndex);
						system_->AddGhostThereIndies(ghostIndex);
					}
				}
			}
		}
	}
}