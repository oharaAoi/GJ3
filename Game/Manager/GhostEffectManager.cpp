#include "GhostEffectManager.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

GhostEffectManager* GhostEffectManager::GetInstance() {
	static GhostEffectManager instance;
	return &instance;
}

GhostEffectManager::~GhostEffectManager() {
	ghostEffect_.clear();
}

void GhostEffectManager::Finalize() {
	ghostEffect_.clear();
}

void GhostEffectManager::Init() {
	ghostEffect_.clear();
	SetName("GhostEffectManager");
	EditorWindows::AddObjectWindow(this, GetName());
}

void GhostEffectManager::Update() {
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

void GhostEffectManager::Draw() const {
	for (const auto& effect : ghostEffect_) {
		effect->Draw();
	}
}

GhostEffect* GhostEffectManager::Create(const Vector2& pos, const Vector2& tileSize) {
	auto& newEffect = ghostEffect_.emplace_back(std::make_unique<GhostEffect>());
	newEffect->Init(tileSize);
	newEffect->SetTranslate(pos);
	return newEffect.get();
}

void GhostEffectManager::Debug_Gui() {
	for (auto& effect : ghostEffect_) {
		effect->Debug_Gui();
	}
}
