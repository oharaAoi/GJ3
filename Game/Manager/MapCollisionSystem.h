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
	// 現状ゴーストを通ったり、墓石を押せなかったりは対応無し。
	// 基本的な動作は大丈夫なはず(何かあったら教えてちょ💛)


	// ====================================================================== //
	//					おばけができているか判定する
	// ====================================================================== //

	void UpdateSpanGhost();
	// 完成してない(デバッグをしていないからバグが起きると思う)

private:

	// 範囲外参照フラグ
	bool OutOfRangeReference(const Vector2Int& index);

	// ステージ上のインデックス番号切り替え用
	void ChengeStage(
		const Vector2Int& direction,
		const Vector2Int& playerIndex
	);

	// ステージ上のインデックスをおばけブロックか判定する
	bool CheckGhostBlock(const Vector2Int& index);
	// pairIndexを入れたらゴーストのIndexの位置が取得できる
	Vector2Int SearchGhostIndex(const Vector2Int& index);

private:

	StageRegistry* stageRegistry_ = nullptr;

	// ゴーストが出来ているペアを保持
	std::vector<Vector2Int> pairIndex_;
	bool ghostUpdate_ = false;

};

