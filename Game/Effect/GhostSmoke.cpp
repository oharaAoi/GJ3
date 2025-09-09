#include "GhostSmoke.h"
#include "Engine/Lib/Math/MyRandom.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

GhostSmoke::~GhostSmoke() {
}

void GhostSmoke::Init(const Vector2& pos, const Vector2& tileSize) {
	SetName("Smoke");
	for (uint32_t oi = 0; oi < 2; ++oi) {
		ghostEffect_[oi] = std::make_unique<GhostEffect>();
		ghostEffect_[oi]->Init(tileSize);
		ghostEffect_[oi]->SetTranslate(pos);

		ghostEffect_[oi]->SetDissolveUvScale(RandomVector3(CVector3::UNIT * -2.0f, CVector3::UNIT * 2.0f));
	}

	animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));
	appearanceAnime_.Init(1.0f, 0.2f, animationItems_.appearDuration, animationItems_.appearEaseType, LoopType::ROUNDTRIP);
	isDestroy_ = false;

	EditorWindows::AddObjectWindow(this, GetName());
}

void GhostSmoke::Update() {
	appearanceAnime_.Update(GameTimer::DeltaTime());

	for (uint32_t oi = 0; oi < 2; ++oi) {
		float value = appearanceAnime_.GetValue();
		ghostEffect_[oi]->SetThreshold(value);
		ghostEffect_[oi]->Update();
	}
}

void GhostSmoke::Draw() const {
	for (uint32_t oi = 0; oi < 2; ++oi) {
		ghostEffect_[oi]->Draw();
	}
}

void GhostSmoke::Debug_Gui() {
	for (uint32_t oi = 0; oi < 2; ++oi) {
		std::string name = "effect_" + std::to_string(oi);
		if (ImGui::TreeNode(name.c_str())) {
			ghostEffect_[oi]->Debug_Gui();
			ImGui::TreePop();
		}
	}

	SelectEasing(animationItems_.appearEaseType);
	ImGui::DragFloat("appearDuration", &animationItems_.appearDuration, 0.01f);
	ImGui::DragFloat("disappearDuration", &animationItems_.disappearDuration, 0.01f);
	if (ImGui::Button("Apperar")) {
		appearanceAnime_.Init(1.0f, 0.2f, animationItems_.appearDuration, animationItems_.appearEaseType, LoopType::ROUNDTRIP);
	}

	if (ImGui::Button("Dissaper")) {
		appearanceAnime_.Init(0.2f, 1.0f, animationItems_.disappearDuration, animationItems_.disappearEaseType, LoopType::ROUNDTRIP);
	}

	ImGui::Separator();
	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), animationItems_.ToJson(animationItems_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));
	}
}
