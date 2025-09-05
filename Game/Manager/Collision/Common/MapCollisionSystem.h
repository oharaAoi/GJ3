#pragma once
#include <array>
#include <vector>
#include <memory>

#include "Engine/Lib/Math/Vector2Int.h"

#include "Game/Manager/StageRegistry.h"
#include "Game/Manager/Collision/Map/SpecialBlockCollision.h"
#include "Game/Manager/Collision/Map/GhostBlockCollision.h"

class MapCollisionSystem
{
public:

	MapCollisionSystem()
		: neighborOffsets_{ 
			Vector2Int{1,0}, Vector2Int{-1,0}, Vector2Int{0,1}, Vector2Int{0,-1},
			Vector2Int{1,1}, Vector2Int{1,-1}, Vector2Int{-1,1}, Vector2Int{-1,-1}
		}
	{}

	void Init(StageRegistry* stageRegistry);

	void Update(); 

	// ====================================================================== //
	//			進めるか判定する(進む方向、プレイヤーインデックス)
	// ====================================================================== //

	bool IsMovable(const Vector2Int& direction, const Vector2Int& playerIndex);

private:

	// ====================================================================== //
	//					おばけができているか判定する
	// ====================================================================== //

	void UpdateSpanGhost();

private:

	// ステージ上のインデックス番号切り替え用
	void ChengeStage(const Vector2Int& direction, const Vector2Int& playerIndex);

public:

	// 範囲外参照フラグ
	bool OutOfRangeReference(const Vector2Int& index);

	// 規制ブロックを検知してゴーストを作成するか判定する
	bool CheckLimitBlock(const Vector2Int& _index);

	// ゴーストができるか判定する(ゴーストブロックのIndex)
	void CheckTokenGhost(const Vector2Int& index){ ghostBlockCollision_->CheckTokenGhost(index); }
	// ゴーストを作成する
	void CreateTokenGhost() { ghostBlockCollision_->CreateTokenGhost(); };
	// pairIndexを入れたらゴーストのIndexの位置が取得できる
	Vector2Int SearchGhostIndex(const Vector2Int& index) { return ghostBlockCollision_->SearchGhostIndex(index); }

	// ====================================================================== //
	//					回収したおばけの数を取得する		
	// ====================================================================== //

	const uint32_t GetGhostCounter()const { return ghostCounter_; }
	void AddGhostCounter();
	void ResetGhostCounter() { ghostCounter_ = 0; }

	// ====================================================================== //
	//								ゲッター	
	// ====================================================================== //

	StageRegistry* GetStageRegi()const { return stageRegistry_; }

	bool GetIsClear() const { return isClear_; }

	const std::array<Vector2Int, 8>& GetNeighborOffsets()const { return neighborOffsets_; }

	const Vector2Int& GetPlayerIndex() { return playerIndex_; }

	// ゴーストブロックのペアIndex
	const std::vector<Vector2Int>& GetPairIndex()const { return ghostBlockCollision_->GetPairIndex(); }
	void AddPairIndex(const Vector2Int index) { ghostBlockCollision_->AddPairIndex(index); }
	void ClearPairIndex() { ghostBlockCollision_->ClearPairIndex(); }

private:

	// ゴーストブロックの判定
	std::unique_ptr<GhostBlockCollision> ghostBlockCollision_ = nullptr;
	// 特殊ブロックの判定
	std::unique_ptr<SpecialBlockCollision> spBlockCollision_ = nullptr;
	// ステージ管理
	StageRegistry* stageRegistry_ = nullptr;

	// プレイヤーのIndex
	Vector2Int playerIndex_ = { 0,0 };
	// 中心から8方向へのoffset
	std::array<Vector2Int, 8> neighborOffsets_;

	// 触れたおばけの数を保持
	uint32_t ghostCounter_ = 0;

	// クリア判定を返す
	bool isClear_ = false;
};

