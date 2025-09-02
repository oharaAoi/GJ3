#include "StageEditor.h"
#include "Engine/Utilities/ImGuiHelperFunc.h"
#include "Engine/Utilities/Logger.h"
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 初期化処理
///////////////////////////////////////////////////////////////////////////////////////////////

void StageEditor::Init() {
	currentIndex_ = 0;
	SetName("StageEditor");
	// フォルダの確認
	if (!fs::exists(kCsvDirectoryPath_)) {
		fs::create_directories(kCsvDirectoryPath_);
	}

	// フォルダ内のファイル名を配列に格納
	filePaths_.clear();
	for (const auto& entry : fs::recursive_directory_iterator(kCsvDirectoryPath_)) {
		if (entry.is_regular_file()) {
			std::string ext = entry.path().extension().string();
			if (ext == ".csv") {
				std::string fileName = entry.path().filename().string();
				filePaths_.push_back(fileName);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 編集処理
///////////////////////////////////////////////////////////////////////////////////////////////

void StageEditor::Debug_Gui() {
	// csvファイルの設定
	ContainerOfComb(filePaths_, currentIndex_, "stageEditor");
	stageInformation_.csvName = filePaths_[currentIndex_];
	if (ImGui::Button("Import")) {
		isChangeStage_ = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("HotReload")) {
		Init();
	}

	// 番号の設定
	if (ImGui::Button("-")) {
		stageInformation_.needGhostNum--;
	}
	ImGui::SameLine();
	ImGui::Text("%d", stageInformation_.needGhostNum);
	ImGui::SameLine();
	if (ImGui::Button("+")) {
		stageInformation_.needGhostNum++;
	}

	// 保存
	if (ImGui::Button("Save")) {
		ImGui::OpenPopup("保存ウィンドウ");
	}

	if (ImGui::BeginPopupModal("保存ウィンドウ", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("stage_");
		ImGui::SameLine();
		ImGui::DragInt("##saveNum", &saveStageNum_);

		if (ImGui::Button("OK")) {
			SaveStageData(stageInformation_.ToJson(stageInformation_.GetName()));
			MessageBoxA(nullptr, "Compleate Save", "Stage Serializer", 0);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ ステージを保存する
///////////////////////////////////////////////////////////////////////////////////////////////

void StageEditor::SaveStageData(const json& _jsonData) {
	if (_jsonData.is_object() && !_jsonData.empty()) {
		// 最上位のキーの名前をファイル名とする
		std::string rootKey = "stage_" + std::to_string(saveStageNum_);
		std::string filePath;
		std::filesystem::path dirPath = filePath = kStageDirectoryPath_ + rootKey + ".json";
		
		// -------------------------------------------------
		// ↓ ディレクトリがなければ作成を行う
		// -------------------------------------------------
		if (!std::filesystem::exists(kStageDirectoryPath_)) {
			std::filesystem::create_directories(kStageDirectoryPath_);
			std::cout << "Created directory: " << kStageDirectoryPath_ << std::endl;
		}

		// -------------------------------------------------
		// ↓ ファイルを開けるかのチェックを行う
		// -------------------------------------------------
		std::ofstream outFile(filePath);
		if (outFile.fail()) {
			std::string message = "Faild open data file for write\n";
			//Log(message);
			assert(0);
			return;
		}

		// -------------------------------------------------
		// ↓ ファイルに実際に書き込む
		// -------------------------------------------------
		outFile << std::setw(4) << _jsonData << std::endl;
		outFile.close();

	} else {
		Logger::AssertLog("not save stageData : stage" + std::to_string(saveStageNum_));
	}
}
