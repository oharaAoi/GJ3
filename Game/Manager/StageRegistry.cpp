#include "StageRegistry.h"
#include "Game/Tool/MapChipLoader.h"
#include "Game/Actor/Block/BlockWall.h"
#include "Game/Actor/Block/BlockFactory.h"

void StageRegistry::Init() {

}

void StageRegistry::Update() {

}

void StageRegistry::Register(const std::string& _fileName) {
	stageData_.clear();
	std::vector<std::vector<uint32_t>> data = MapChipLoader::Load(kDirectoryPath_, _fileName);
	size_t rows = data.size();
	size_t cols = data[0].size();

	// サイズの決定
	stageData_.clear();
	stageData_.resize(rows);
	for (auto& row : stageData_) {
		row.resize(cols);
	}

	for (size_t row = 0; row < rows; ++row) {
		for (size_t col = 0; col < cols; ++col) {
			stageData_[row][col] = CreateBlock(data[row][col]);
			IBlock* newBlock = stageData_[row][col].get();
			if (newBlock != nullptr) {
				newBlock->Init();

				// 位置を決定
				Vector2 pos = Vector2((128.f) * col, (128.f) * row);
				newBlock->SetPosition(pos);

			}
		}
	}
}

