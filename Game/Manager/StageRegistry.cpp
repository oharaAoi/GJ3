#include "StageRegistry.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Game/Tool/MapChipLoader.h"
#include "Game/Actor/Block/BlockWall.h"
#include "Game/Actor/Block/BlockFactory.h"
#include "Game/Information/StageInformation.h"
#include "Enviroment.h"

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
		CreatesMap(stageEditor_->GetStageName());
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

	// mapの修正
	CreatesMap(information.csvName);
}

void StageRegistry::CreatesMap(const std::string& _csvFileName) {
	// stageの情報を登録
	DestroyData();
	stageData_.clear();
	std::vector<std::vector<uint32_t>> data = MapChipLoader::Load(kDirectoryPath_, _csvFileName);
	maxSize_.x = static_cast<int>(data[0].size());
	maxSize_.y = static_cast<int>(data.size());

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
			// 共通して設定する
			if (newBlock != nullptr) {
				newBlock->Init();
				newBlock->SetIndex(Vector2Int{ static_cast<int>(row),static_cast<int>(col) });

				// 位置を決定
				newBlock->SetPosition(CalculateTilePos(row, col));
				newBlock->GetSprite()->ReSetTextureSize(tileSize_);
			}

			// 特別な情報の取得
			if (data[row][col] == static_cast<int>(BlockType::Player)) {
				startIndex_ = { (int)col, (int)row };
				startPos_ = CalculateTilePos(row, col);

			} else if (data[row][col] == static_cast<int>(BlockType::Goal)) {
				goalIndex_ = { (int)col, (int)row };
			}
		}
	}
}

void StageRegistry::SetStageData(const Vector2Int& index, std::unique_ptr<IBlock> block) {
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

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ タイルの座標を計算
///////////////////////////////////////////////////////////////////////////////////////////////

Vector2 StageRegistry::CalculateTilePos(size_t row, size_t col) {
	// 大体の横縦のタイルの数
	const int baseCols = 12;
	const int baseRows = 10;
	// どのくらいの大きさ可の割合
	float raito = 0.7f;

	// タイルのサイズを画面の比率から計算
	tileSize_ = Vector2(static_cast<float>(kWindowWidth_ * raito) / baseCols, static_cast<float>(kWindowHeight_ * raito) / baseRows);

	// scalringの大きさをマップの縦横の大きさから計算
	float scaleX = static_cast<float>(baseCols) / maxSize_.x;
	float scaleY = static_cast<float>(baseRows) / maxSize_.y;
	float scale = std::min(scaleX, scaleY); // 縦横比を維持

	// ベースのサイズを求める
	float baseSize = std::min(tileSize_.x, tileSize_.y) * scale;
	tileSize_ = Vector2(baseSize, baseSize);

	// マップの大きさを計算
	Vector2 mapSize = Vector2(tileSize_.x * maxSize_.x, tileSize_.y * maxSize_.y);
	mapOffset_ = Vector2(
		(static_cast<float>(kWindowWidth_) - mapSize.x) / 2.0f,
		(static_cast<float>(kWindowHeight_) - mapSize.y) / 2.0f
	);
	Vector2 centerPos = Vector2(
		mapOffset_.x + col * tileSize_.x + tileSize_.x / 2.0f,
		mapOffset_.y + row * tileSize_.y + tileSize_.y / 2.0f
	);
	return centerPos;
}

