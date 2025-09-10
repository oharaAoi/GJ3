#include "LimitBlockEffectManager.h"

LimitBlockEffectManager::~LimitBlockEffectManager() {
	Finalize();
}

LimitBlockEffectManager* LimitBlockEffectManager::GetInstance() {
	static LimitBlockEffectManager instance;
	return &instance;
}

void LimitBlockEffectManager::Finalize() {
	limitBlockEffect_.clear();
}

void LimitBlockEffectManager::Init() {
	SetName("LimitBlockEffectManager");
	limitBlockEffect_.clear();
}

void LimitBlockEffectManager::Update() {
	for (auto it = limitBlockEffect_.begin(); it != limitBlockEffect_.end(); ) {
		if ((*it)->GetIsDestroy()) {
			it = limitBlockEffect_.erase(it);
		} else {
			++it;
		}
	}

	for (auto& effect : limitBlockEffect_) {
		effect->Update();
	}
}

void LimitBlockEffectManager::Draw() const {
	for (const auto& effect : limitBlockEffect_) {
		effect->Draw();
	}
}

LimitBlockEffect* LimitBlockEffectManager::Create(const Vector2& pos, const Vector2& tileSize) {
	for (const auto& effect : limitBlockEffect_) {
		Vector2 effectPos = effect->GetPos();
		if (pos.x == effectPos.x && pos.y == effectPos.y) {
			return nullptr;
		}
	}

	auto& newEffect = limitBlockEffect_.emplace_back(std::make_unique<LimitBlockEffect>());
	newEffect->Init(pos, tileSize);
	return newEffect.get();
}


void LimitBlockEffectManager::Debug_Gui() {
}