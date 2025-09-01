#pragma once
#include <vector>
#include <string>
#include <cstdint>

/// <summary>
/// マップを読み込むクラス
/// </summary>
class MapChipLoader {
public:

	MapChipLoader() = default; 
	~MapChipLoader() = default;

	static std::vector<std::vector<uint32_t>> Load(const std::string& rootPath, const std::string& fileName);


};

