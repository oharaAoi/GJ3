#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Lib/Math/Vector2Int.h"
#include "Game/Actor/Block/IBlock.h"
#include "Game/Information/BlockType.h"
#include "Game/Tool/StageEditor.h"
#include "Game/Tool/StageLoader.h"

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

	void Register(const std::string& _fileName);

	const std::vector<std::vector<std::unique_ptr<IBlock>>>& GetStageData() { return stageData_; }
	void SetStageData(const Vector2Int& index, std::unique_ptr<IBlock> block);
	void DestroyData();

	void Debug_Gui() override;

private:

	const std::string kDirectoryPath_ = "./Game/Assets/GameData/Map/Csv/";
	std::vector<std::vector<std::unique_ptr<IBlock>>> stageData_;

	Vector2Int maxSize_ = Vector2Int(0, 0);

	std::unique_ptr<StageEditor> stageEditor_;
	std::unique_ptr<StageLoader> stageLoader_;

	int needGhostNum_;
};

