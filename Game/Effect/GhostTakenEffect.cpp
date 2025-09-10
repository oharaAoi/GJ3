#include "GhostTakenEffect.h"
#include "Engine/Lib/Math/MyRandom.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

GhostTakenEffect::~GhostTakenEffect() {
}

void GhostTakenEffect::Init(const Vector2& pos, const Vector2& tileSize) {
	SetName("GhostTakenEffect");
	position_ = pos;
	for (uint32_t oi = 0; oi < 2; ++oi) {
		ghostEffect_[oi] = std::make_unique<GhostTakenSwirl>();
		ghostEffect_[oi]->Init(tileSize);
		ghostEffect_[oi]->SetTranslate(pos);

		ghostEffect_[oi]->SetDissolveUvScale(RandomVector3(CVector3::UNIT * -2.0f, CVector3::UNIT * 2.0f));
	}

	animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));
	isDestroy_ = false;
	lifeTimer_ = 0;

	EditorWindows::AddObjectWindow(this, GetName());
}

void GhostTakenEffect::Update() {
	lifeTimer_ += GameTimer::DeltaTime();

	for (uint32_t oi = 0; oi < 1; ++oi) {
		ghostEffect_[oi]->Update();
	}

	/*if (lifeTimer_ >= animationItems_.duration) {
		isDestroy_ = true;
	}*/
}

void GhostTakenEffect::Draw() const {
	for (uint32_t oi = 0; oi < 1; ++oi) {
		ghostEffect_[oi]->Draw();
	}
}

void GhostTakenEffect::ApplySaveData(const std::string& effectName) {
	for (uint32_t oi = 0; oi < 1; ++oi) {
		ghostEffect_[oi]->ApplySaveData(effectName);
	}
}

void GhostTakenEffect::Debug_Gui() {
	for (uint32_t oi = 0; oi < 1; ++oi) {
		std::string name = "effect_" + std::to_string(oi);
		if (ImGui::TreeNode(name.c_str())) {
			ghostEffect_[oi]->Debug_Gui();
			ImGui::TreePop();
		}
	}
}
