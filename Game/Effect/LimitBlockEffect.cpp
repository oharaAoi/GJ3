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

	ghostEffect_ = std::make_unique<GhostEffect>();
	ghostEffect_->Init(tileSize);
	ghostEffect_->SetTranslate(pos);

	ghostEffect_->SetDissolveUvScale(RandomVector3(CVector3::UNIT * -1.0f, CVector3::UNIT * 1.0f));

	Vector3 min = RandomVector3(CVector3::UNIT * -1.0f, CVector3::UNIT * 1.0f);
	Vector3 max = RandomVector3(CVector3::UNIT * 1.0f, CVector3::UNIT * 1.0f);
	uvAnimation_.Init(min, max, animationItems_.effectDuration, (int)EasingType::None::Liner, LoopType::RETURN);

	effectAlpha_.Init(0.5f, 1.0f, 1.5f, (int)EasingType::None::Liner, LoopType::RETURN);
	isDestroy_ = false;

	animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));
	EditorWindows::AddObjectWindow(this, GetName());
}

void LimitBlockEffect::Update() {
	uvAnimation_.Update(GameTimer::DeltaTime());
	effectAlpha_.Update(GameTimer::DeltaTime());
	ghostEffect_->SetDissolveUvTranslate(uvAnimation_.GetValue());
	ghostEffect_->SetAlpha(effectAlpha_.GetValue());
	ghostEffect_->Update();
}

void LimitBlockEffect::Draw() const {
	ghostEffect_->Draw();
}

void LimitBlockEffect::ApplySaveData(const std::string& effectName) {
	ghostEffect_->ApplySaveData(effectName);
}

void LimitBlockEffect::SetPos(const Vector2& pos) {
	ghostEffect_->SetTranslate(pos);
}

void LimitBlockEffect::Debug_Gui() {

	std::string name = "effect_";
	if (ImGui::TreeNode(name.c_str())) {
		ghostEffect_->Debug_Gui();
		ImGui::TreePop();
	}

	std::string duration = "duration_";
	ImGui::DragFloat(duration.c_str(), &animationItems_.effectDuration, 0.01f);


	if (ImGui::Button("Replay")) {

		Vector3 min = RandomVector3(CVector3::UNIT * -1.0f, CVector3::UNIT * 1.0f);
		Vector3 max = RandomVector3(CVector3::UNIT * 1.0f, CVector3::UNIT * 1.0f);
		uvAnimation_.Init(min, max, animationItems_.effectDuration, (int)EasingType::None::Liner, LoopType::RETURN);
	}

	ImGui::Separator();
	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), animationItems_.ToJson(animationItems_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));
	}

}