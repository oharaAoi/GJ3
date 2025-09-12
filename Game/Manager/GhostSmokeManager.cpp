#include "GhostSmokeManager.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

GhostSmokeManager* GhostSmokeManager::GetInstance() {
	static GhostSmokeManager instance;
	return &instance;
}

GhostSmokeManager::~GhostSmokeManager() {
	ghostEffect_.clear();
}

void GhostSmokeManager::Finalize() {
	ghostEffect_.clear();
}

void GhostSmokeManager::Init() {
	SetName("GhostSmokeManager");
	ghostEffect_.clear();
}

void GhostSmokeManager::Update() {
	for (auto it = ghostEffect_.begin(); it != ghostEffect_.end(); ) {
		if ((*it)->GetIsDestroy()) {
			it = ghostEffect_.erase(it);
		} else {
			++it;
		}
	}

	for (auto& effect : ghostEffect_) {
		effect->Update();
	}
}

void GhostSmokeManager::Draw() const {
	for (const auto& effect : ghostEffect_) {
		effect->Draw();
	}
}

std::shared_ptr<GhostSmoke> GhostSmokeManager::Create(const Vector2& pos, const Vector2& tileSize) {
	auto& newEffect = ghostEffect_.emplace_back(std::make_shared<GhostSmoke>());
	newEffect->Init(pos, tileSize);
	newEffect->ApplySaveData("GhostTakenSwirl");
	return newEffect;
}

void GhostSmokeManager::Debug_Gui() {
	
}
