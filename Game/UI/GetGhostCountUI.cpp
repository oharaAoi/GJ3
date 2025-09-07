#include "GetGhostCountUI.h"
#include "Engine/Lib/Math/MyMath.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void GetGhostCountUI::Init(Canvas2d* _canvas2d) {
	SetName("GetGhostCountUI");
	pCanvas2d_ = _canvas2d;

	param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));

	auto& newNowUI = nowCount_.emplace_back(pCanvas2d_->AddSprite("number.png", "nowCount", "Sprite_Normal.json"));
	newNowUI->SetDrawRange(param_.numberSize);
	newNowUI->ReSetTextureSize(param_.numberSize);
	auto& newTargetUI = targetCount_.emplace_back(pCanvas2d_->AddSprite("number.png", "nowCount", "Sprite_Normal.json"));
	newTargetUI->SetDrawRange(param_.numberSize);
	newTargetUI->ReSetTextureSize(param_.numberSize);

	barSprite_ = pCanvas2d_->AddSprite("bar.png", "bar", "Sprite_Normal.json");

	EditorWindows::AddObjectWindow(this, GetName());
}

void GetGhostCountUI::Update(uint32_t _nowCount, uint32_t targetCount) {
	// 現在の桁数を求める
	uint32_t nowDegit = DegitCount(_nowCount);
	// カウントと配列の要素数が一致しないときは配列に代入
	if (nowDegit > nowCount_.size()) {
		auto& newUI = nowCount_.emplace_back(pCanvas2d_->AddSprite("number.png", "nowCount", "Sprite_Normal.json"));
		newUI->SetDrawRange(param_.numberSize);
		newUI->ReSetTextureSize(param_.numberSize);
	}

	nowDegit = DegitCount(targetCount);
	if (nowDegit > targetCount_.size()) {
		auto& newTargetUI = targetCount_.emplace_back(pCanvas2d_->AddSprite("number.png", "nowCount", "Sprite_Normal.json"));
		newTargetUI->SetDrawRange(param_.numberSize);
		newTargetUI->ReSetTextureSize(param_.numberSize);
	}
	
	// spriteの変更
	for (int oi = 0; oi < nowCount_.size(); ++oi) {
		// 現在の数字を求める
		uint32_t integer = IntegerCount(_nowCount, oi + 1);
		// 左上の座標を求める
		Vector2 leftTop = NumberSpriteLt(integer, numberSize_);
		// 座標を変える
		nowCount_[oi]->SetLeftTop(leftTop);

		Vector2 pos = { param_.nowPos.x + (param_.numberSize.x * oi),  param_.nowPos.y };
		nowCount_[oi]->SetTranslate(pos);
	}

	for (int oi = 0; oi < targetCount_.size(); ++oi) {
		// 現在の数字を求める
		uint32_t integer = IntegerCount(targetCount, oi + 1);
		// 左上の座標を求める
		Vector2 leftTop = NumberSpriteLt(integer, numberSize_);
		// 座標を変える
		targetCount_[oi]->SetLeftTop(leftTop);

		Vector2 pos = { param_.targetPos.x + (param_.numberSize.x * oi),  param_.targetPos.y };
		targetCount_[oi]->SetTranslate(param_.targetPos);
	}

	barSprite_->SetTranslate(param_.barPos);
}

void GetGhostCountUI::Debug_Gui() {
	ImGui::DragFloat2("nowPos", &param_.nowPos.x, 0.1f);
	ImGui::DragFloat2("targetPos", &param_.targetPos.x, 0.1f);
	ImGui::DragFloat2("barPos", &param_.barPos.x, 0.1f);
	ImGui::DragFloat2("numberSize", &param_.numberSize.x, 0.1f);

	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));
	}
}
