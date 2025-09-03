#pragma once
#include <vector>

#include "StageRegistry.h"

class MapCollisionSystem
{
public:

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

	// 範囲外参照フラグ
	bool OutOfRangeReference(const Vector2Int& index);

	// ステージ上のインデックス番号切り替え用
	void ChengeStage(const Vector2Int& direction, const Vector2Int& playerIndex);

	// ステージ上のインデックスをおばけブロックか判定する
	bool CheckGhostBlock(const Vector2Int& playerIndex, const Vector2Int& index);

	// pairIndexを入れたらゴーストのIndexの位置が取得できる
	Vector2Int SearchGhostIndex(const Vector2Int& index);

	// ゴーストが墓石に変わる
	void ChangeGrave(const Vector2Int& index);

public:

	// ====================================================================== //
	//					回収したおばけの数を取得する		
	// ====================================================================== //

	const uint32_t GetGhostCounter()const { return ghostCounter_; }
	void ResetGhostCounter() { ghostCounter_ = 0; }

private:

	StageRegistry* stageRegistry_ = nullptr;

	// プレイヤーのIndex
	Vector2Int playerIndex_ = { 0,0 };

	// おばけが出来ているペアを保持
	std::vector<Vector2Int> pairIndex_;
	bool ghostUpdate_ = false;

	// 触れたおばけの数を保持
	uint32_t ghostCounter_ = 0;

};

