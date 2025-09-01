#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "Engine/Module/Components/Attribute/AttributeGui.h"

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

private:

	const std::string kDirectoryPath_ = "./Game/Assets/GameData/Map/";
	std::vector<std::string> filePaths_;

	std::string currentFileName_;
	int currentIndex_;
};

