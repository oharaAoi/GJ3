#include "GhostSoulManager.h"

#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Engine/Lib/GameTimer.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 初期化処理
///////////////////////////////////////////////////////////////////////////////////////////////

void GhostSoulManager::Init(Canvas2d* _canvas2d, Player* _player) {
	pCanvas2d_ = _canvas2d;
	pPlayer_ = _player;

	souls_.clear();
}

void GhostSoulManager::InitClearScene(Canvas2d* _canvas2d)
{
	pCanvas2d_ = _canvas2d;
	souls_.clear();

	SetName("GhostSoulManager");
	AddChild(this);
	EditorWindows::AddObjectWindow(this, GetName());

	param_.FromJson(JsonItems::GetData("ClearSoul", param_.GetName()));
}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 更新
///////////////////////////////////////////////////////////////////////////////////////////////

void GhostSoulManager::Update() {
	for (auto& soul : souls_) {
		soul->Update(pPlayer_->GetPosition());
	}
}

void GhostSoulManager::UpdateClearScene()
{
	angleRad_ += GameTimer::DeltaTime();
	if (kPI2 < angleRad_) { angleRad_ = 0.0f; }
	for (size_t i = 0; i < souls_.size(); ++i) {
		float nextAngle = kPI2 / static_cast<float>(souls_.size()) * static_cast<float>(i);
		nextAngle += angleRad_;
		// サイズを更新
		Vector2 def_size = { param_.def_size,param_.def_size };
		Vector2 change_size = { param_.size,param_.size };
		def_size += change_size * std::sin(nextAngle);
		souls_[i]->SetTextureSize(def_size);
		// 座標を更新
		Vector2 pos = {};
		pos.x = param_.center.x + param_.radius.x * std::cos(nextAngle);
		pos.y = param_.center.y + param_.radius.y * std::sin(nextAngle);
		souls_[i]->Update(pos);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 作成処理
///////////////////////////////////////////////////////////////////////////////////////////////

void GhostSoulManager::CreateSoul(const Vector2& tileSize) {
	auto& newSoul = souls_.emplace_back(std::make_unique<GhostSoul>());
	newSoul->Init(pCanvas2d_, tileSize);
}

void GhostSoulManager::SetPosition(const Vector2& position) {
	for (auto& soul : souls_) { soul->SetPosition(position); }
}

void GhostSoulManager::Debug_Gui()
{
	ImGui::DragFloat2("center", &param_.center.x);
	ImGui::DragFloat2("radius", &param_.radius.x);
	ImGui::DragFloat("def_size", &param_.def_size);
	ImGui::DragFloat("size", &param_.size);

	if (ImGui::Button("Save")) {
		JsonItems::Save("ClearSoul", param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		param_.FromJson(JsonItems::GetData("ClearSoul", param_.GetName()));
	}
}
