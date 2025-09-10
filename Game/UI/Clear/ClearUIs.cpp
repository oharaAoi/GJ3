#include "ClearUIs.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

#include "Game/UI/Button/SelectButtonUI.h"
#include "Game/UI/Button/NextStageButtonUI.h"

void ClearUIs::Init()
{
	SetName("ClearUIs");
	EditorWindows::AddObjectWindow(this, GetName());

	param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));
	// ボタン生成、初期化
	buttonUIs_[0] = std::make_unique<SelectButtonUI>();
	buttonUIs_[1] = std::make_unique<NextStageButtonUI>();

	// 初期化
	for (size_t i = 0; i < buttonUIs_.size(); ++i) {
		buttonUIs_[i]->Init();
		Vector2 position = param_.selectButtonPos;
		if (i == 1) { position = param_.nextStageButtonPos; }
		buttonUIs_[i]->GetSprite()->SetTranslate(position);
	}
	buttonUIs_[0]->GetSprite()->SetTextureName("Clear_selectUI.png");
	buttonUIs_[0]->GetSprite()->ReSetTextureSize(buttonUIs_[1]->GetSprite()->GetTextureSize());

	// Clear_text
	text_ = Engine::GetCanvas2d()->AddSprite("Clear_text.png", "Text", "Sprite_Normal.json", 1, true);
	AddChild(text_);
	// Clear_pot
	pot_ = Engine::GetCanvas2d()->AddSprite("Clear_pot.png","Pot", "Sprite_Normal.json", 1, true);
	AddChild(pot_);

	// Clear_curtain
	curtain_ = Engine::GetCanvas2d()->AddSprite("Clear_curtain.png", "Curtain", "Sprite_Normal.json", 1, true);
	AddChild(curtain_);
	// Clear_bg
	backGround_ = Engine::GetCanvas2d()->AddSprite("Clear_bg.png", "BackGround", "Sprite_Normal.json", 0, true);
	AddChild(backGround_);

	text_->SetTranslate(Vector2{ 640.0f,360.0f });
	pot_->SetTranslate(Vector2{ 640.0f,360.0f });
	curtain_->SetTranslate(Vector2{ 640.0f,360.0f });
	backGround_->SetTranslate(Vector2{ 640.0f,360.0f });
}

void ClearUIs::Update()
{
	for (size_t i = 0; i < buttonUIs_.size(); ++i) {
		Vector2 position = param_.selectButtonPos;
		if (i == 1) { position = param_.nextStageButtonPos; }
		buttonUIs_[i]->GetSprite()->SetTranslate(position);
	}
}


void ClearUIs::Debug_Gui()
{
	ImGui::DragFloat2("selectButtonPos", &param_.selectButtonPos.x, 0.1f);
	ImGui::DragFloat2("nextStageButtonPos", &param_.nextStageButtonPos.x, 0.1f);

	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));
	}
}
