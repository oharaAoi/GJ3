#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Engine/Lib/Json/IJsonConverter.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

/// <summary>
/// Stageの情報を読み込んでくれるクラス
/// </summary>
class StageLoader :
	public AttributeGui {
public:

	StageLoader() = default;
	~StageLoader() = default;

	void Init();

	json Load(const std::string& jsonFile);

	void Debug_Gui() override;

	const std::string GetStageName() const { return currentFileName_; }

private:

	const std::string kStageDirectoryPath_ = "./Game/Assets/GameData/Map/Stage/";
	std::vector<std::string> filePaths_;

	std::string currentFileName_;
	int currentIndex_;

};

