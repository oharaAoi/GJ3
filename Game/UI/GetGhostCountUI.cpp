#include "GetGhostCountUI.h"
#include "Engine/Lib/Math/MyMath.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void GetGhostCountUI::Init(Canvas2d* _canvas2d) {
	SetName("GetGhostCountUI");
	pCanvas2d_ = _canvas2d;

	param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));

	auto& newTargetUI = targetCount_.emplace_back(pCanvas2d_->AddSprite("number.png", "nowCount", "Sprite_Normal.json", 1));
	newTargetUI->SetDrawRange(param_.numberSize);
	newTargetUI->ReSetTextureSize(param_.numberSize);

	crystalSprite_ = pCanvas2d_->AddSprite("crystal.png", "bar", "Sprite_Normal.json");

	EditorWindows::AddObjectWindow(this, GetName());
}

void GetGhostCountUI::Update(uint32_t _nowCount, uint32_t targetCount) {
	// 現在の桁数を求める
	uint32_t nowDegit = DegitCount(_nowCount);

	nowDegit = DegitCount(targetCount);
	if (nowDegit > targetCount_.size()) {
		auto& newTargetUI = targetCount_.emplace_back(pCanvas2d_->AddSprite("number.png", "nowCount", "Sprite_Normal.json"));
		newTargetUI->SetDrawRange(param_.numberSize);
		newTargetUI->ReSetTextureSize(param_.numberSize);
	}

	for (int oi = 0; oi < targetCount_.size(); ++oi) {
		// 現在の数字を求める
		uint32_t integer = IntegerCount(targetCount - _nowCount, oi + 1);
		int count = static_cast<int>(targetCount - _nowCount);
		if (count < 0) { 
			integer = 0; 
		}
		// 左上の座標を求める
		Vector2 leftTop = NumberSpriteLt(integer, numberSize_);
		// 座標を変える
		targetCount_[oi]->SetLeftTop(leftTop);
		targetCount_[oi]->SetDrawRange(param_.numberSize);
		targetCount_[oi]->ReSetTextureSize(param_.numberSize);

		Vector2 pos = param_.targetPos;
		Vector2 size = param_.numberSize * 0.25f;
		if (targetCount_.size() != 1) {
			if (oi == 0) { pos.x += size.x; }
			else { pos.x -= size.x; }
		}
		targetCount_[oi]->SetTranslate(pos);
		targetCount_[oi]->SetColor(param_.color);
	}

	crystalSprite_->SetTranslate(param_.barPos);
}

void GetGhostCountUI::Debug_Gui() {
	ImGui::DragFloat2("nowPos", &param_.nowPos.x, 0.1f);
	ImGui::DragFloat2("targetPos", &param_.targetPos.x, 0.1f);
	ImGui::DragFloat2("barPos", &param_.barPos.x, 0.1f);
	ImGui::DragFloat2("numberSize", &param_.numberSize.x, 0.1f);
	ImGui::ColorEdit4("numberColor", &param_.color.x);

	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));
	}
}
