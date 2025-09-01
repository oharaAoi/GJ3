#include "Canvas2d.h"
#include "Engine.h"

Canvas2d::~Canvas2d() {
}

void Canvas2d::Init() {
	spriteList_.clear();
}

void Canvas2d::Update() {
	for (auto it = spriteList_.begin(); it != spriteList_.end(); ) {
		if ((*it).sprite->GetIsDestroy()) {
			it = spriteList_.erase(it);
		} else {
			++it;
		}
	}

	spriteList_.sort([](const ObjectPair& a, const ObjectPair& b) {
		if (a.renderQueue == b.renderQueue) {
			return a.renderQueue < b.renderQueue;
		}
		return a.renderQueue == b.renderQueue;
					 });

	for (auto& it : spriteList_) {
		if (it.sprite->GetEnable()) {
			it.sprite->Update();
		}
	}
}

void Canvas2d::Draw() const {
	for (const auto& it : spriteList_) {
		if (it.sprite->GetEnable()) {
			Pipeline* pso = Engine::SetPipeline(PSOType::Sprite, it.psoName);
			it.sprite->Draw(pso);
		}
	}
}

void Canvas2d::EditObject(const ImVec2& windowSize, const ImVec2& imagePos) {
	for (const auto& it : spriteList_) {
		if (it.sprite->GetEnable()) {
			it.sprite->GetTransform()->Manipulate(windowSize, imagePos);
		}
	}
}

Sprite* Canvas2d::AddSprite(const std::string& textureName, const std::string& psoName, int renderQueue) {
	auto& newObj = spriteList_.emplace_back(ObjectPair());
	newObj.sprite = std::make_unique<Sprite>();
	newObj.sprite->Init(textureName);
	newObj.psoName = psoName;
	newObj.renderQueue = renderQueue;
	
	return newObj.sprite.get();
}
