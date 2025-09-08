#pragma once
#include <vector>

#include "Engine/Lib/Math/Vector2Int.h"

class MapCollisionSystem;

class GhostBlockCollision
{
public:

	void Init(MapCollisionSystem* system);


	// ゴーストを作成する
	void CreateTokenGhost();

	// おばけができるか判定する(ゴーストブロックのIndex)
	void CheckTokenGhost(const Vector2Int& index);

	// pairIndexを入れたらゴーストのIndexの位置が取得できる
	Vector2Int SearchGhostIndex(const Vector2Int& index);

	// ゴーストが墓石に変わる
	void ChangeGrave(const Vector2Int& index);

public:

	// ゴーストをマップに発生させるか
	const bool GetGhostUpdate()const { return ghostUpdate_; }
	const bool GetCreateGhost()const { return isCreateGhost_; }
	void SetGhostUpdate(bool flag) { ghostUpdate_ = flag; }

	// ゴーストブロックのペアIndex
	const std::vector<Vector2Int>& GetPairIndex()const { return pairIndex_; }
	void AddPairIndex(const Vector2Int index) { pairIndex_.push_back(index); }
	void ClearPairIndex() { pairIndex_.clear(); }

private:

	// ステージ上のインデックスをおばけブロックか判定する
	bool CheckGhostBlock(const Vector2Int& playerIndex, const Vector2Int& index);

private:

	MapCollisionSystem* system_ = nullptr;

	// おばけが出来ているペアを保持
	std::vector<Vector2Int> pairIndex_;
	// ゴーストをマップに発生させるか
	bool ghostUpdate_ = false;
	bool isCreateGhost_ = false;

};
