#include "GhostTakenEffect.h"
#include "Engine/Lib/Math/MyRandom.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

GhostTakenEffect::~GhostTakenEffect() {
}

void GhostTakenEffect::Init(const Vector2& pos, const Vector2& tileSize) {
	SetName("GhostTakenEffect");
	position_ = pos;

	ghostEffect_ = std::make_unique<GhostTakenSwirl>();
	ghostEffect_->Init(tileSize);
	ghostEffect_->SetTranslate(pos);
	ghostEffect_->SetDissolveUvScale(RandomVector3(CVector3::UNIT * -2.0f, CVector3::UNIT * 2.0f));

	animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));
	isDestroy_ = false;
	isScaleUp_ = true;
	lifeTimer_ = 0;

	EditorWindows::AddObjectWindow(this, GetName());
}

void GhostTakenEffect::Update() {
	lifeTimer_ += GameTimer::DeltaTime();

	if (isScaleUp_) {
		ScaleUp();
	} else {
		ScaleDown();
	}
	ghostEffect_->Update();

}

void GhostTakenEffect::Draw() const {
	ghostEffect_->Draw();
}

void GhostTakenEffect::ApplySaveData(const std::string& effectName) {
	ghostEffect_->ApplySaveData(effectName);
}

void GhostTakenEffect::ScaleUp() {
	if (lifeTimer_ <= animationItems_.scaleUpTime) {
		float t = lifeTimer_ / animationItems_.scaleUpTime;
		Vector2 scale = Vector2::Lerp(CVector2::ZERO, CVector2::UNIT * 1.5f, Ease::Out::Quart(t));
		ghostEffect_->SetScale(scale);
	} else {
		lifeTimer_ = 0;
		isScaleUp_ = false;
	}
}

void GhostTakenEffect::ScaleDown() {
	if (lifeTimer_ <= animationItems_.scaleDownTime) {
		float t = lifeTimer_ / animationItems_.scaleDownTime;
		Vector2 scale = Vector2::Lerp(CVector2::UNIT * 1.5f, CVector2::ZERO, Ease::In::Back(t));
		ghostEffect_->SetScale(scale);
	} else {
		isDestroy_ = true;
	}
}

void GhostTakenEffect::Debug_Gui() {
	std::string name = "effect";
	if (ImGui::Button("Replay")) {
		isScaleUp_ = true;
		lifeTimer_ = 0;
	}
	if (ImGui::TreeNode(name.c_str())) {
		ghostEffect_->Debug_Gui();
		ImGui::TreePop();
	}

	ImGui::DragFloat("scaleDownTime", &animationItems_.scaleDownTime, 0.01f);
	ImGui::DragFloat("scaleUpTime", &animationItems_.scaleUpTime, 0.01f);
	ImGui::Separator();
	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), animationItems_.ToJson(animationItems_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));
	}
}
