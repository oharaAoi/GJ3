#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Lib/Json/IJsonConverter.h"
#include "Game/Information/StageInformation.h"

/// <summary>
/// Stageを編集するクラス
/// </summary>
class StageEditor :
	public AttributeGui{

public:

	StageEditor() = default;
	~StageEditor() override = default;

	void Init();

	void Debug_Gui() override;

	void SaveStageData(const json& _jsonData);

	void SetIsChangeStage(bool _flag) { isChangeStage_ = _flag; }
	bool GetIsChangeStage() { return isChangeStage_; };

	std::string GetStageName() { return stageInformation_.csvName; }

private:

	const std::string kCsvDirectoryPath_ = "./Game/Assets/GameData/Map/Csv/";
	const std::string kStageDirectoryPath_ = "./Game/Assets/GameData/Map/Stage/";
	std::vector<std::string> filePaths_;

	std::string currentFileName_;
	int currentIndex_;

	StageInformation stageInformation_;
	int saveStageNum_;

	bool isChangeStage_;
};

