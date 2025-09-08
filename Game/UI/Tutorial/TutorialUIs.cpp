#include "TutorialUIs.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Lib/Math/Easing.h"

#include "Game/UI/Tutorial/Text/Self/TextUI_Self_Line1.h"
#include "Game/UI/Tutorial/Text/Self/TextUI_Self_Line2.h"

#include "Game/UI/Tutorial/Text/Move/TextUI_Move_Line1.h"
#include "Game/UI/Tutorial/Text/Move/TextUI_Move_Line2.h"
#include "Game/UI/Tutorial/Text/Move/TextUI_Move_Line3.h"

#include "Game/UI/Tutorial/Text/CreateGhost/TextUI_CreateGhost_Line1.h"
#include "Game/UI/Tutorial/Text/CreateGhost/TextUI_CreateGhost_Line2.h"

#include "Game/UI/Tutorial/Text/Explanation/TextUI_Explanation_Line1.h"
#include "Game/UI/Tutorial/Text/Explanation/TextUI_Explanation_Line2.h"

#include "Game/UI/Tutorial/Text/CollisionGhost/TextUI_CollisionGhost_Line1.h"
#include "Game/UI/Tutorial/Text/CollisionGhost/TextUI_CollisionGhost_Line2.h"

#include "Game/UI/Tutorial/Text/Goal/TextUI_Goal_Line1.h"
#include "Game/UI/Tutorial/Text/Goal/TextUI_Goal_Line2.h"

void TutorialUIs::Init()
{
	SetName("TutorialUIs");
	AddChild(this);
	EditorWindows::AddObjectWindow(this, GetName());
	param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));

	cat_ = Engine::GetCanvas2d()->AddSprite("Tutorial_cat.png", GetName(), "Sprite_Normal.json", 100);
	speechBubble_ = Engine::GetCanvas2d()->AddSprite("Tutorial_speechBubble.png", GetName(), "Sprite_Normal.json", 100);
	buttonA_ = Engine::GetCanvas2d()->AddSprite("Tutorial_A.png", GetName(), "Sprite_Normal.json", 200);
	spaceKey_ = Engine::GetCanvas2d()->AddSprite("Tutorial_SpaceKey.png", GetName(), "Sprite_Normal.json", 200);
	buttonA_->SetTranslate(param_.buttonA_pos);
	buttonA_->SetEnable(false);
	spaceKey_->SetTranslate(param_.spaceKey_pos);
	spaceKey_->SetEnable(false);

	cat_->SetTranslate(Vector2{ 640.0f,360.0f });
	speechBubble_->SetTranslate(Vector2{ 640.0f,360.0f });
	speechBubble_->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.0f });

	// 自己紹介UI
	selfUIs_[0] = std::make_unique<TextUI_Self_Line1>();
	selfUIs_[1] = std::make_unique<TextUI_Self_Line2>();
	selfUIs_[0]->Init();
	selfUIs_[1]->Init();
	// 移動UI
	moveUIs_[0] = std::make_unique<TextUI_Move_Line1>();
	moveUIs_[1] = std::make_unique<TextUI_Move_Line2>();
	moveUIs_[2] = std::make_unique<TextUI_Move_Line3>();
	moveUIs_[0]->Init();
	moveUIs_[1]->Init();
	moveUIs_[2]->Init();
	// ゴーストを作らせるUI
	createUIs_[0] = std::make_unique<TextUI_CreateGhost_Line1>();
	createUIs_[1] = std::make_unique<TextUI_CreateGhost_Line2>();
	createUIs_[0]->Init();
	createUIs_[1]->Init();
	// ゴーストが出来る解説UI
	explanaUIs_[0] = std::make_unique<TextUI_Explanation_Line1>();
	explanaUIs_[1] = std::make_unique<TextUI_Explanation_Line2>();
	explanaUIs_[0]->Init();
	explanaUIs_[1]->Init();
	// ゴーストを取らせるUI
	collisionUIs_[0] = std::make_unique<TextUI_CollisionGhost_Line1>();
	collisionUIs_[1] = std::make_unique<TextUI_CollisionGhost_Line2>();
	collisionUIs_[0]->Init();
	collisionUIs_[1]->Init();
	// ゴールさせるUI
	goalUIs_[0] = std::make_unique<TextUI_Goal_Line1>();
	goalUIs_[1] = std::make_unique<TextUI_Goal_Line2>();
	goalUIs_[0]->Init();
	goalUIs_[1]->Init();
}

void TutorialUIs::Update()
{
	buttonA_->SetTranslate(param_.buttonA_pos);
	spaceKey_->SetTranslate(param_.spaceKey_pos);
}

void TutorialUIs::Debug_Gui()
{
	ImGui::DragFloat2("buttonA_pos", &param_.buttonA_pos.x, 0.1f);
	ImGui::DragFloat2("spaceKey_pos", &param_.spaceKey_pos.x, 0.1f);

	if (ImGui::Button("Save")) {
		JsonItems::Save(GetName(), param_.ToJson(param_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));
	}
}
