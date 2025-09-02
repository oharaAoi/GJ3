#pragma once
#include <string>
#include "Engine/Lib/Json/IJsonConverter.h"
#include "Engine/Lib/Math/Vector2.h"

struct StageInformation : public IJsonConverter {
	std::string csvName;	// ステージのcsv名
	int needGhostNum;		// ゴールに必要なおばけの数

	StageInformation() { SetName("StageInformation"); }

	json ToJson(const std::string& id) const override {
		return JsonBuilder(id)
			.Add("csvName", csvName)
			.Add("needGhostNum", needGhostNum)
			.Build();
	}

	void FromJson(const json& jsonData) override {
		fromJson(jsonData, "csvName", csvName);
		fromJson(jsonData, "needGhostNum", needGhostNum);
	}
};