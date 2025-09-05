#include "Canvas2d.h"
#include "Engine.h"

Canvas2d::~Canvas2d() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Canvas2d::Init() {
	spriteList_.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Canvas2d::Update() {
	for (auto it = spriteList_.begin(); it != spriteList_.end(); ) {
		if ((*it).sprite->GetIsDestroy()) {
			it = spriteList_.erase(it);
		} else {
			++it;
		}
	}

	spriteList_.sort([](const ObjectPair& a, const ObjectPair& b) {
		return a.renderQueue < b.renderQueue;
					 });

	for (auto& it : spriteList_) {
		if (it.sprite->GetEnable()) {
			it.sprite->Update();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Canvas2d::PreDraw() const {
	for (const auto& it : spriteList_) {
		if (it.isPreDraw) {
			if (it.sprite->GetEnable()) {
				Pipeline* pso = Engine::SetPipeline(PSOType::Sprite, "Sprite_Normal_16.json");
				it.sprite->Draw(pso);
			}
		}
	}
}

void Canvas2d::Draw() const {
	for (const auto& it : spriteList_) {
		if (!it.isPreDraw) {
			if (it.sprite->GetEnable()) {
				Pipeline* pso = Engine::SetPipeline(PSOType::Sprite, it.psoName);
				it.sprite->Draw(pso);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Canvas2d::EditObject(const ImVec2& windowSize, const ImVec2& imagePos) {
	for (const auto& it : spriteList_) {
		if (it.sprite->GetEnable()) {
			it.sprite->GetTransform()->Manipulate(windowSize, imagePos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　追加処理
//////////////////////////////////////////////////////////////////////////////////////////////////

Sprite* Canvas2d::AddSprite(const std::string& _textureName, const std::string& _attributeName, const std::string& _psoName, int _renderQueue, bool _isPreDraw) {
	auto& newObj = spriteList_.emplace_back(ObjectPair());
	newObj.sprite = std::make_unique<Sprite>();
	newObj.sprite->Init(_textureName);
	newObj.sprite->SetName(_attributeName);
	newObj.psoName = _psoName;
	newObj.renderQueue = _renderQueue;
	newObj.isPreDraw = _isPreDraw;
	
	return newObj.sprite.get();
}
