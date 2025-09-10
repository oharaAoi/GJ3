#include "GhostTakenEffectManager.h"

GhostTakenEffectManager::~GhostTakenEffectManager() {
}

GhostTakenEffectManager* GhostTakenEffectManager::GetInstance() {
	static GhostTakenEffectManager instance;
	return &instance;
}

void GhostTakenEffectManager::Finalize() {
	ghostTakenEffect_.clear();
}

void GhostTakenEffectManager::Init() {
	SetName("GhostTakenEffectManager");
	ghostTakenEffect_.clear();
}

void GhostTakenEffectManager::Update() {
	for (auto it = ghostTakenEffect_.begin(); it != ghostTakenEffect_.end(); ) {
		if ((*it)->GetIsDestroy()) {
			it = ghostTakenEffect_.erase(it);
		} else {
			++it;
		}
	}

	for (auto& effect : ghostTakenEffect_) {
		effect->Update();
	}
}

void GhostTakenEffectManager::Draw() const {
	for (const auto& effect : ghostTakenEffect_) {
		effect->Draw();
	}
}

void GhostTakenEffectManager::Create(const Vector2& pos, const Vector2& tileSize) {
	for (const auto& effect : ghostTakenEffect_) {
		Vector2 effectPos = effect->GetPos();
		if (pos.x == effectPos.x && pos.y == effectPos.y) {
			return;
		}
	}

	auto& newEffect = ghostTakenEffect_.emplace_back(std::make_unique<GhostTakenEffect>());
	newEffect->Init(pos, tileSize);
	newEffect->ApplySaveData("GhostEffect");
}

void GhostTakenEffectManager::Debug_Gui() {
}