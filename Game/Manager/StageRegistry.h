#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Game/Actor/Block/IBlock.h"
#include "Game/Information/BlockType.h"

/// <summary>
/// Stageの情報を所有しているクラス
/// </summary>
class StageRegistry {
public:

	StageRegistry() = default;
	~StageRegistry() = default;

	void Init();

	void Update();

	void Register(const std::string& _fileName);

private:

	const std::string kDirectoryPath_ = "./Game/Assets/GameData/Map/";
	std::vector<std::vector<std::unique_ptr<IBlock>>> stageData_;

};

