#include "StageLoader.h"
#include <iostream>
#include <fstream>
#include "Engine/Utilities/ImGuiHelperFunc.h"

uint32_t StageLoader::maxStageNum_ = 0;

namespace fs = std::filesystem;

void StageLoader::Init() {
	SetName("StageLoader");
	// フォルダの確認
	if (!fs::exists(kStageDirectoryPath_)) {
		fs::create_directories(kStageDirectoryPath_);
	}

	// フォルダ内のファイル名を配列に格納
	filePaths_.clear();
	maxStageNum_ = 0;
	for (const auto& entry : fs::recursive_directory_iterator(kStageDirectoryPath_)) {
		if (entry.is_regular_file()) {
			std::string ext = entry.path().extension().string();
			if (ext == ".json") {
				std::string fileName = entry.path().filename().string();
				filePaths_.push_back(fileName);

				maxStageNum_++;
			}
		}
	}
}

json StageLoader::Load(const std::string& jsonFile) {
	std::string filePath = kStageDirectoryPath_ + jsonFile;
	
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + filePath + ".json";
		return json();
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// 値の追加
	return root;
}

void StageLoader::Debug_Gui() {
	if (filePaths_.empty()) { return; }
	ContainerOfComb(filePaths_, currentIndex_, "stageEditor");
	currentFileName_ = filePaths_[currentIndex_];
}
