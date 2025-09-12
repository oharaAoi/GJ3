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

std::shared_ptr<LimitBlockEffect> LimitBlockEffectManager::Create(const Vector2& pos, const Vector2& tileSize) {
	auto newEffect = limitBlockEffect_.emplace_back(std::make_shared<LimitBlockEffect>());
	newEffect->Init(pos, tileSize);
	return newEffect;
}


void LimitBlockEffectManager::Debug_Gui() {
}