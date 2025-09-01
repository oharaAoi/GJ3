#include "MapChipLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Engine/Utilities/Logger.h"

std::vector<std::vector<uint32_t>> MapChipLoader::Load(const std::string& rootPath, const std::string& fileName) {
	std::vector<std::vector<uint32_t>> data;
	std::ifstream file(rootPath + fileName);
	if (!file.is_open()) {
		Logger::AssertLog("Dont Find MapData : " + fileName);
	}

	std::string line;
	while (std::getline(file, line)) {
		std::vector<uint32_t> row;
		std::stringstream ss(line);
		std::string cell;

		while (std::getline(ss, cell, ',')) {
			row.push_back(static_cast<uint32_t>(std::stoul(cell)));
		}
		data.push_back(row);
	}

	return data;
}
