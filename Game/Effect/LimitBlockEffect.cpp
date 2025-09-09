#include "LimitBlockEffect.h"
#include "Engine/Lib/Math/MyRandom.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

LimitBlockEffect::~LimitBlockEffect() {
}

void LimitBlockEffect::Init(const Vector2& pos, const Vector2& tileSize) {
	SetName("LimitBlockEffect");
	position_ = pos;
	animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));

	for (uint32_t oi = 0; oi < 2; ++oi) {
		ghostEffect_[oi] = std::make_unique<GhostEffect>();
		ghostEffect_[oi]->Init(tileSize);
		ghostEffect_[oi]->SetTranslate(pos);

		ghostEffect_[oi]->SetDissolveUvScale(RandomVector3(CVector3::UNIT * -2.0f, CVector3::UNIT * 2.0f));

		ghostEffect_[oi]->SetDissolveUvScale(RandomVector3(CVector3::UNIT * 3.0f, CVector3::UNIT * 6.0f));

		Vector3 min = RandomVector3(CVector3::UNIT * -1.0f, CVector3::UNIT * 1.0f);
		Vector3 max = RandomVector3(CVector3:: UNIT * 1.0f, CVector3::UNIT * 1.0f);
		uvAnimation_[oi].Init(min, max, animationItems_.effectDuration[oi], (int)EasingType::None::Liner, LoopType::RETURN);
	}

	isDestroy_ = false;

	animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));
	EditorWindows::AddObjectWindow(this, GetName());
}

void LimitBlockEffect::Update() {
	for (uint32_t oi = 0; oi < 2; ++oi) {
		uvAnimation_[oi].Update(GameTimer::DeltaTime());
		ghostEffect_[oi]->SetDissolveUvTranslate(uvAnimation_[oi].GetValue());
		ghostEffect_[oi]->Update();
	}
}

void LimitBlockEffect::Draw() const {
	for (uint32_t oi = 0; oi < 2; ++oi) {
		ghostEffect_[oi]->Draw();
	}
}

void LimitBlockEffect::ApplySaveData(const std::string& effectName) {
	for (uint32_t oi = 0; oi < 2; ++oi) {
		ghostEffect_[oi]->ApplySaveData(effectName);
	}
}

void LimitBlockEffect::Debug_Gui() {
	for (uint32_t oi = 0; oi < 2; ++oi) {
		std::string name = "effect_" + std::to_string(oi);
		if (ImGui::TreeNode(name.c_str())) {
			ghostEffect_[oi]->Debug_Gui();
			ImGui::TreePop();
		}

		std::string duration = "duration_" + std::to_string(oi);
		ImGui::DragFloat(duration.c_str(), &animationItems_.effectDuration[oi], 0.01f);
	}

	if (ImGui::Button("Replay")) {
		for (uint32_t oi = 0; oi < 2; ++oi) {
			Vector3 min = RandomVector3(CVector3::UNIT * -1.0f, CVector3::UNIT * 1.0f);
			Vector3 max = RandomVector3(CVector3::UNIT * 1.0f, CVector3::UNIT * 1.0f);
			uvAnimation_[oi].Init(min, max, animationItems_.effectDuration[oi], (int)EasingType::None::Liner, LoopType::RETURN);
		}
	}

	ImGui::Separator();
	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), animationItems_.ToJson(animationItems_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));
	}

}