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
	lifeTimer_ = 0;

	EditorWindows::AddObjectWindow(this, GetName());
}

void GhostTakenEffect::Update() {
	lifeTimer_ += GameTimer::DeltaTime();
	// 生存時間の確認
	if (lifeTimer_ >= animationItems_.duration) {
		isDestroy_ = true;
	}
	// スケールを小さくする
	float t = lifeTimer_ / animationItems_.duration;
	Vector2 scale = Vector2::Lerp(CVector2::UNIT, CVector2::ZERO, Ease::In::Back(t));
	ghostEffect_->SetScale(scale);
	ghostEffect_->Update();

}

void GhostTakenEffect::Draw() const {
	ghostEffect_->Draw();
}

void GhostTakenEffect::ApplySaveData(const std::string& effectName) {
	ghostEffect_->ApplySaveData(effectName);
}

void GhostTakenEffect::Debug_Gui() {
	std::string name = "effect";
	if (ImGui::TreeNode(name.c_str())) {
		ghostEffect_->Debug_Gui();
		ImGui::TreePop();
	}

	ImGui::DragFloat("duration", &animationItems_.duration, 0.01f);
	ImGui::Separator();
	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), animationItems_.ToJson(animationItems_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		animationItems_.FromJson(JsonItems::GetData(GetName(), animationItems_.GetName()));
	}
}
