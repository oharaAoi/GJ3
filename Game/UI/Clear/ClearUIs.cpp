#include "ClearUIs.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

#include "Game/UI/Button/SelectButtonUI.h"
#include "Game/UI/Button/NextStageButtonUI.h"

void ClearUIs::Init()
{
	SetName("ClearUIs");
	AddChild(this);
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
