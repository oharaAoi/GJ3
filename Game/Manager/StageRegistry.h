#pragma once
#include <vector>
#include <string>
#include <memory>
// Engine
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Lib/Math/Vector2Int.h"
// Game
#include "Game/Actor/Block/IBlock.h"
#include "Game/Information/BlockType.h"
#include "Game/Tool/StageEditor.h"
#include "Game/Tool/StageLoader.h"
#include "Game/Actor/Player/Player.h"

/// <summary>
/// Stageの情報を所有しているクラス
/// </summary>
class StageRegistry : 
	public AttributeGui {
public:

	StageRegistry() = default;
	~StageRegistry() = default;

	void Init();

	void Update();

	void Register(const std::string& _jsonFileName);

	void CreatesMap(const std::string& _csvFileName);

	void ClearStageData(const Vector2Int& index);

	void DestroyData();

	void Debug_Gui() override;

public:

	const std::vector<std::vector<std::unique_ptr<IBlock>>>& GetStageData() { return stageData_; }

	void SetStageData(const Vector2Int& index, const Vector2Int& assignIndex);
	void SetGhostData(const Vector2Int& index);

	void SetPlayer(Player* _player) { pPlayer_ = _player; }

	const Vector2& GetTileSize() const { return tileSize_; }
	const Vector2& GetStartPos() const { return startPos_; }
	const Vector2& GetMapOffset() const { return mapOffset_; }
	const Vector2Int& GetStartIndex() const { return startIndex_; }
	const Vector2Int& GetGoalIndex() const { return goalIndex_; }

private:

	Vector2 CalculateTilePos(size_t row, size_t col);

	void ResetPlayer(); // playerの位置と情報をリセットする

private:

	const std::string kDirectoryPath_ = "./Game/Assets/GameData/Map/Csv/";
	std::vector<std::vector<std::unique_ptr<IBlock>>> stageData_;

	std::unique_ptr<StageEditor> stageEditor_;
	std::unique_ptr<StageLoader> stageLoader_;

	// その他クラスのポインタ
	Player* pPlayer_ = nullptr;

	// Stageに関連する情報変数
	Vector2Int maxSize_ = Vector2Int(0, 0);
	Vector2 tileSize_;
	int needGhostNum_;

	Vector2Int startIndex_ = Vector2Int(0, 0);
	Vector2 startPos_;
	Vector2 mapOffset_;

	Vector2Int goalIndex_ = Vector2Int(0, 0);
};

