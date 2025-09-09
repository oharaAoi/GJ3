#include "MapCollisionSystem.h"
/// engine
#include "Engine/System/AUdio/AudioPlayer.h"

/// commands
#include "Game/Commands/ObjectCommandInvoker.h"
#include "Game/Commands/Stage/MoveBlockCommand.h"

void MapCollisionSystem::Init(StageRegistry* stageRegistry,GhostSoulManager* ghostSoulManager){
	stageRegistry_ = stageRegistry;
	pGhostSoulManager_ = ghostSoulManager;

	ghostBlockCollision_ = std::make_unique<GhostBlockCollision>();
	ghostBlockCollision_->Init(this);
	// 特殊ブロッククラスの初期化
	spBlockCollision_ = std::make_unique<SpecialBlockCollision>();
	spBlockCollision_->Init(this);

	// 初期化時に一回だけ更新をかけてゴーストを反映させる
	playerIndex_ = stageRegistry_->GetStartIndex();
	UpdateSpanGhost();

	isClear_ = false;
}

void MapCollisionSystem::Update(){
	UpdateSpanGhost();
}

bool MapCollisionSystem::IsMovable(const Vector2Int& direction,const Vector2Int& playerIndex){
	// プレイヤーと進む方向のblockTypeとindexを取得
	const auto& data = stageRegistry_->GetStageData();

	// プレイヤーブロック
	Vector2Int index = playerIndex + direction;
	// 範囲外か見る
	if(!OutOfRangeReference(index)){
		return false;
	}

	// 進む方向1マス目ブロック
	IBlock* firstStepIndex = data[index.y][index.x].get();
	// 何もないなら進める
	if(firstStepIndex == nullptr){
		return true;
	}

	// ゴールと当たった時
	if(firstStepIndex->GetType() == BlockType::Goal &&
	   stageRegistry_->GetNeedGhostNum() <= ghostCounter_){
		isClear_ = true;
		return true;
	}

	// ghoustと当たったた時
	if(firstStepIndex->GetType() == BlockType::Ghost){
		ghostBlockCollision_->ChangeGrave(index);
		// おばけが変わったなら更新を入れる
		//spBlockCollision_->ChangeBlock();
		ghostBlockCollision_->SetGhostUpdate(true);
		return true;
	}
	// 壁だから進めない
	if(firstStepIndex->GetType() == BlockType::Wall ||
	   firstStepIndex->GetType() == BlockType::GraveBlock ||
	   firstStepIndex->GetType() == BlockType::Goal){
		AudioPlayer::SinglShotPlay("stop.mp3",0.6f); // 壁にあたったときの音
		return false;
	}

	// 進む方向2マス目ブロック
	index += direction;
	if(!OutOfRangeReference(index)){
		AudioPlayer::SinglShotPlay("stop.mp3",0.6f);
		return false;
	}
	IBlock* secondStepIndex = data[index.y][index.x].get();
	// 1マス目が動かせるブロックで2マス目がないなら早期return
	if(firstStepIndex->GetType() == BlockType::NormalBlock || firstStepIndex->GetType() == BlockType::GhostBlock ||
	   firstStepIndex->GetType() == BlockType::SpecialBlock || firstStepIndex->GetType() == BlockType::LimitBlock){
		if(secondStepIndex == nullptr || secondStepIndex->GetType() == BlockType::Ghost){
			
			playerIndex_ = playerIndex + direction;
			ChengeStage(direction,playerIndex);
			return true;
		}
	}
	AudioPlayer::SinglShotPlay("stop.mp3",0.6f); // 壁にあたったときの音
	return false;
}

void MapCollisionSystem::UpdateSpanGhost(){
	if(!ghostBlockCollision_->GetGhostUpdate()){
		return;
	}

	// ステージデータからゴーストブロックを取得
	const auto& data = stageRegistry_->GetStageData();
	ClearPairIndex();
	for(const auto& row : data){
		for(const auto& col : row){
			// 特殊ブロックは判定しない
			if(col == nullptr){
				continue;
			} else{
				col->SetIsChengeBlock(false);
			}
			if(col->GetIsSpecialBlock()){
				continue;
			}
			// おばけをリセットする
			if(col->GetType() == BlockType::Ghost){
				stageRegistry_->ClearStageData(col->GetIndex());
				continue;
			}
			// ゴーストブロックじゃ無ければコンティニュー
			if(col->GetType() != BlockType::GhostBlock){
				continue;
			}
			// ゴーストブロックだったら
			Vector2Int index = col->GetIndex();
			ghostBlockCollision_->CheckTokenGhost(index);
		}
	}
	// 実際のおばけ生成ポイント
	ghostBlockCollision_->CreateTokenGhost();

	// 特殊ブロックの入り判定のゴーストを作成する処理
	ClearPairIndex();
	for(const auto& row : data){
		for(const auto& col : row){
			if(col == nullptr){
				continue;
			}
			spBlockCollision_->RecursionBlockChecker(col->GetIndex());
		}
	}
	// 特殊ブロックの出判定を取る
	spBlockCollision_->ChangeBlock();

	// 全ゴーストの生成をする
	ClearPairIndex();
	for(const auto& row : data){
		for(const auto& col : row){
			// nullは判定しない
			if(col == nullptr ||
			   col->GetType() != BlockType::GhostBlock){
				continue;
			}
			// ゴーストブロックだったら
			Vector2Int index = col->GetIndex();
			ghostBlockCollision_->CheckTokenGhost(index);
		}
	}
	// 実際のおばけ生成ポイント
	ghostBlockCollision_->CreateTokenGhost();

	// 今フレームで新しくおばけが作成されたかどうかでEffectの生成処理判定を行う
	for (const auto& nowFrame : ghostThereIndies_) {
		// 過去フレームの配列が空だったら問答無用で作成
		if (preFrameGhostThereIndies_.empty()) {
			stageRegistry_->CreateGhostEffect(nowFrame);
			continue;	// 下の処理で作成しないように次の要素へ
		}
		// 前フレームのindexの配列になかったら作成する
		bool isExit = false;
		for (const auto& preFrame : preFrameGhostThereIndies_) {
			if (nowFrame == preFrame) {
				isExit = true;
				break;
			}
		}

		// なかったので作成
		if (!isExit) {
			stageRegistry_->CreateGhostEffect(nowFrame);
		}
	}
	preFrameGhostThereIndies_.clear();
	preFrameGhostThereIndies_ = ghostThereIndies_;
	ghostThereIndies_.clear();
}

void MapCollisionSystem::AddGhostThereIndies(const Vector2Int& index) {
	/*if (ghostThereIndies_.empty()) {
		ghostThereIndies_.push_back(index);
	}
	for (const auto& nowFrame : ghostThereIndies_) {
		if (nowFrame != index) {
			ghostThereIndies_.push_back(index);
		}
	}*/

	ghostThereIndies_.push_back(index);
}

void MapCollisionSystem::AddPreGhostThereIndies(const Vector2Int& index) {
	preFrameGhostThereIndies_.push_back(index);
}

bool MapCollisionSystem::OutOfRangeReference(const Vector2Int& index){
	const auto& data = stageRegistry_->GetStageData();
	size_t row = data.size();
	size_t col = data[0].size();
	// 範囲外では無いか検出
	if(index.y >= 0 && index.y < row && index.x >= 0 && index.x < col){
		return true;
	}
	return false;
}

void MapCollisionSystem::ChengeStage(const Vector2Int& direction,const Vector2Int& playerIndex){
	// プレイヤーの元居たIndex番号からうめていく
	Vector2Int index = playerIndex;
	Vector2Int firstStepIndex = index + direction;
	Vector2Int secondStepIndex = firstStepIndex + direction;

	auto moveBlockCommand = std::make_unique<MoveBlockCommand>(stageRegistry_,this,firstStepIndex,secondStepIndex);
	moveBlockCommand->Execute();
	ObjectCommandInvoker::GetInstance().PushCommand(std::move(moveBlockCommand));

	GetGhostBlockCollision()->SetGhostUpdate(true);
}

bool MapCollisionSystem::CheckLimitBlock(const Vector2Int& _index){
	// 8方向検出する
	const auto& data = stageRegistry_->GetStageData();
	for(auto& index : neighborOffsets_){
		Vector2Int nextIndex = index + _index;
		if(data[nextIndex.y][nextIndex.x] == nullptr){
			continue;
		}
		if(data[nextIndex.y][nextIndex.x]->GetType() == BlockType::LimitBlock){
			return false;
		}
	}
	return true;
}

void MapCollisionSystem::AddGhostCounter(){
	++ghostCounter_;
	AudioPlayer::SinglShotPlay("ghost_get.mp3",0.6f); // ゴースト獲得時の音

	if(pGhostSoulManager_){
		pGhostSoulManager_->CreateSoul(stageRegistry_->GetTileSize());
	}
}

void MapCollisionSystem::SubGhostCounter(){
	--ghostCounter_;

	if(pGhostSoulManager_){
		pGhostSoulManager_->DeleteBackSoul();
	}
}
