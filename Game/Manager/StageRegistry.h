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

	void Init(Canvas2d* _canvas2d);

	void Update();

	void ResetStage();

	void Register(const std::string& _fileName);

	void CreatesStageByMapData();

	const std::vector<std::vector<std::unique_ptr<IBlock>>>& GetStageData() { return stageData_; }
	std::vector<std::vector<std::unique_ptr<IBlock>>>& GetStageDataRef() { return stageData_; }

	void CreateStageData(const Vector2Int& index, BlockType type);
	void ClearStageData(const Vector2Int& index);

	void CreateGhostEffect(const Vector2Int& index);

	void DestroyStageData();
	void DestroyMapData();
	void DestroyData();

	void Debug_Gui() override;

public:

	void SetPlayer(Player* _player) { pPlayer_ = _player; }
	void SetWindowSize(const Vector2& _size) { windowSize_ = _size; }
	void SetTileRatio(float _ratio) { tileRatio_ = _ratio; }

	const Vector2& GetTileSize() const { return tileSize_; }
	const Vector2& GetStartPos() const { return startPos_; }
	const Vector2& GetMapOffset() const { return mapOffset_; }
	const Vector2Int& GetStartIndex() const { return startIndex_; }
	const Vector2Int& GetGoalIndex() const { return goalIndex_; }

	uint32_t GetNeedGhostNum() const { return needGhostNum_; }

	uint32_t GetMaxStageNum() const { return stageLoader_->GetMaxStageNum(); }

	Vector2 CalculateTilePos(size_t row, size_t col);

private:

	void ResetPlayer(); // playerの位置と情報をリセットする

private:

	const std::string kDirectoryPath_ = "./Game/Assets/GameData/Map/Csv/";
	std::vector<std::vector<std::unique_ptr<IBlock>>> stageData_;
	// CSVデータを一時的に保存する変数
	std::vector<std::vector<uint32_t>> mapData_;

	std::unique_ptr<StageEditor> stageEditor_;
	std::unique_ptr<StageLoader> stageLoader_;
	Sprite* startBlock_ = nullptr;

	// その他クラスのポイン
	Canvas2d* pCanvas2d_;
	Player* pPlayer_ = nullptr;

	// Stageに関連する情報変数
	Vector2 windowSize_;
	Vector2Int maxSize_ = Vector2Int(0,0);
	Vector2 tileSize_;
	uint32_t needGhostNum_;

	Vector2Int startIndex_ = Vector2Int(0,0);
	Vector2 startPos_;
	Vector2 mapOffset_;

	Vector2Int goalIndex_ = Vector2Int(0,0);

	float tileRatio_ = 0.7f;
};

