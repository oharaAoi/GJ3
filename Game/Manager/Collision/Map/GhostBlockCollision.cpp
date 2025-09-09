#include "GhostBlockCollision.h"

#include "Game/Manager/Collision/Common/MapCollisionSystem.h"
#include "Game/Manager/StageRegistry.h"

/// command
#include "Game/Commands/ObjectCommandInvoker.h"
#include "Game/Commands/Stage/PlayerGetGhostCommand.h"
#include "Game/Commands/Stage/CreateGraveBlock.h"

void GhostBlockCollision::Init(MapCollisionSystem* system){
	system_ = system;
	pairIndex_.clear();
	ghostUpdate_ = true;
}

bool GhostBlockCollision::CheckGhostBlock(const Vector2Int& playerIndex,const Vector2Int& index){
	const auto& data = system_->GetStageRegi()->GetStageData();
	// 1マス目の検知
	Vector2Int firstStepIndex = playerIndex + index;
	if(!system_->OutOfRangeReference(firstStepIndex)){ return false; }
	// 何も無いか判定する
	if(data[firstStepIndex.y][firstStepIndex.x] != nullptr){ return false; }
	// 2マス目の検知
	// 範囲外では無いか検出
	Vector2Int secondStepIndex = firstStepIndex + index;
	if(!system_->OutOfRangeReference(secondStepIndex)){ return false; }
	if(data[secondStepIndex.y][secondStepIndex.x] == nullptr){ return false; }
	// ゴーストブロックか判定
	if(ghostUpdate_ && data[secondStepIndex.y][secondStepIndex.x]->GetIsSpecialBlock()){ return false; }
	if(data[secondStepIndex.y][secondStepIndex.x]->GetType() == BlockType::GhostBlock){
		return true;
	}
	return false;
}

void GhostBlockCollision::CreateTokenGhost(){
	// 実際のおばけ生成ポイント
	const auto& data = system_->GetStageRegi()->GetStageData();
	for(auto& pair : pairIndex_){
		Vector2Int ghostIndex = SearchGhostIndex(pair);
		if(data[ghostIndex.y][ghostIndex.x] == nullptr ||
		   (data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::NormalBlock &&
		   data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::GhostBlock &&
		   data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::SpecialBlock &&
		   data[ghostIndex.y][ghostIndex.x]->GetType() != BlockType::LimitBlock)){
			// ブロックを押したタイミングでゴーストに当たっているなら
			if(system_->CheckLimitBlock(ghostIndex)){
				const auto& playerIndex = system_->GetPlayerIndex();
				if(ghostIndex.x == playerIndex.x && ghostIndex.y == playerIndex.y){
					// count を増やすコマンド & 墓石を作るコマンド を積む
					auto PlayerGetGhostCommandCommand = std::make_unique<PlayerGetGhostCommand>(system_,playerIndex);
					ObjectCommandInvoker::GetInstance().PushCommand(std::move(PlayerGetGhostCommandCommand));

					auto createGraveCommand = std::make_unique<CreateGraveBlock>(system_->GetStageRegi(),ghostIndex);
					createGraveCommand->Execute();
					ObjectCommandInvoker::GetInstance().PushCommand(std::move(createGraveCommand));
				} else{
					system_->GetStageRegi()->CreateStageData(ghostIndex,BlockType::Ghost);
					isCreateGhost_ = true;
					system_->AddGhostThereIndies(ghostIndex);
				}
			}
		}
	}

	ghostUpdate_ = false;
}

void GhostBlockCollision::CheckTokenGhost(const Vector2Int& index){
	const auto& data = system_->GetStageRegi()->GetStageData();
	const size_t row_size = data[0].size();
	// ゴーストができるか判定する
	// できるペアをpairIndexに保存する
	for(size_t i = 0; i < (system_->GetNeighborOffsets().size() / 2); ++i){
		if(CheckGhostBlock(index,system_->GetNeighborOffsets()[i])){
			// ゴーストの出来る座標の間を取得する
			Vector2Int secondStepIndex = system_->GetNeighborOffsets()[i] + system_->GetNeighborOffsets()[i] + index;
			Vector2Int ghostIndex = {
				index.x * static_cast<int>(row_size) + index.y,
				secondStepIndex.x * static_cast<int>(row_size) + secondStepIndex.y,
			};
			bool hitPair = false;
			for(auto& pair : pairIndex_){
				if((pair.x == ghostIndex.x && pair.y == ghostIndex.y) ||
				   (pair.y == ghostIndex.x && pair.x == ghostIndex.y)){
					hitPair = true;
					continue;
				}
			}
			if(hitPair){ continue; }
			pairIndex_.push_back(ghostIndex);
		}
	}
}

Vector2Int GhostBlockCollision::SearchGhostIndex(const Vector2Int& index){
	const auto& data = system_->GetStageRegi()->GetStageData();
	int col = static_cast<int>(data[0].size());
	return Vector2Int{
		((index.x + index.y) / 2) / col,
		((index.x + index.y) / 2) % col
	};
}

void GhostBlockCollision::ChangeGrave(const Vector2Int& index){
	const auto& data = system_->GetStageRegi()->GetStageData();
	if(data[index.y][index.x] == nullptr){ return; }
	if(data[index.y][index.x]->GetType() == BlockType::Ghost){
		auto createGraveCommand = std::make_unique<CreateGraveBlock>(system_->GetStageRegi(),index);
		createGraveCommand->Execute();
		ObjectCommandInvoker::GetInstance().PushCommand(std::move(createGraveCommand));

		auto PlayerGetGhostCommandCommand = std::make_unique<PlayerGetGhostCommand>(system_,index);
		ObjectCommandInvoker::GetInstance().PushCommand(std::move(PlayerGetGhostCommandCommand));
	}
}