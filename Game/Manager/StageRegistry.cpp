#include "StageRegistry.h"
#include "Game/Tool/MapChipLoader.h"
#include "Game/Actor/Block/BlockWall.h"
#include "Game/Actor/Block/BlockFactory.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Game/Information/StageInformation.h"

void StageRegistry::Init() {
	SetName("StageRegistry");

	stageEditor_ = std::make_unique<StageEditor>();
	stageEditor_->Init();
	AddChild(stageEditor_.get());

	stageLoader_ = std::make_unique<StageLoader>();
	stageLoader_->Init();

	EditorWindows::AddObjectWindow(this, GetName());
}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 更新処理
///////////////////////////////////////////////////////////////////////////////////////////////

void StageRegistry::Update() {
#ifdef _DEBUG
	if (stageEditor_->GetIsChangeStage()) {
		DestroyData();

		Register(stageEditor_->GetStageName());
		stageEditor_->SetIsChangeStage(false);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ Stageの登録
///////////////////////////////////////////////////////////////////////////////////////////////

void StageRegistry::Register(const std::string& _fileName) {
	// jsonファイルの解析をする
	json stageData = stageLoader_->Load(_fileName);
	if (!stageData.is_object() && stageData.empty()) { return; }

	// jsonファイルから構造体にデータを移す
	StageInformation information;
	information.FromJson(stageData);

	// 構造体の情報を登録
	needGhostNum_ = information.needGhostNum;

	// stageの情報を登録
	DestroyData();
	stageData_.clear();
	std::vector<std::vector<uint32_t>> data = MapChipLoader::Load(kDirectoryPath_, information.csvName);
	maxSize_.x = static_cast<int>(data.size());
	maxSize_.y = static_cast<int>(data[0].size());

	// サイズの決定
	stageData_.clear();
	stageData_.resize(maxSize_.y);
	for (auto& row : stageData_) {
		row.resize(maxSize_.x);
	}

	for (size_t row = 0; row < maxSize_.y; ++row) {
		for (size_t col = 0; col < maxSize_.x; ++col) {
			stageData_[row][col] = CreateBlock(data[row][col]);
			IBlock* newBlock = stageData_[row][col].get();
			if (newBlock != nullptr) {
				newBlock->Init();
				newBlock->SetIndex(Vector2Int{ static_cast<int>(row),static_cast<int>(col) });

				// 位置を決定
				Vector2 pos = Vector2((128.f) * col, (128.f) * row);
				newBlock->SetPosition(pos);
			}
		}
	}
}

void StageRegistry::SetStageData(const Vector2Int& index, std::unique_ptr<IBlock> block)
{
	stageData_[index.x][index.y] = std::move(block);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ シーンからポインタを削除する
///////////////////////////////////////////////////////////////////////////////////////////////

void StageRegistry::DestroyData() {
	for (size_t row = 0; row < maxSize_.y; ++row) {
		for (size_t col = 0; col < maxSize_.x; ++col) {
			if (stageData_[row][col] != nullptr) {
				stageData_[row][col]->Destroy();
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 編集処理
///////////////////////////////////////////////////////////////////////////////////////////////

void StageRegistry::Debug_Gui() {
	stageLoader_->Debug_Gui();
	if (ImGui::Button("Import")) {
		Register(stageLoader_->GetStageName());
	}
	ImGui::SameLine();
	if (ImGui::Button("HotReload")) {
		stageLoader_->Init();
	}
}

