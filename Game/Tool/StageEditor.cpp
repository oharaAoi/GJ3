#include "StageEditor.h"
#include "Engine/Utilities/ImGuiHelperFunc.h"

namespace fs = std::filesystem;

void StageEditor::Init() {
	currentIndex_ = 0;
	SetName("StageEditor");
	// フォルダの確認
	if (!fs::exists(kDirectoryPath_)) {
		fs::create_directories(kDirectoryPath_);
	}

	// フォルダ内のファイル名を配列に格納
	for (const auto& entry : fs::recursive_directory_iterator(kDirectoryPath_)) {
		if (entry.is_regular_file()) {
			std::string ext = entry.path().extension().string();
			if (ext == ".csv") {
				std::string fileName = entry.path().filename().string();
				filePaths_.push_back(fileName);
			}
		}
	}
}

void StageEditor::Debug_Gui() {
	ContainerOfComb(filePaths_, currentIndex_, "stageEditor");
}
