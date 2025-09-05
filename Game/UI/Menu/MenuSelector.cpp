#include "MenuSelector.h"

#include "Engine/System/Input/Input.h"

#include "Game/UI/Menu/button/MenuButtonType.h"

void MenuSelector::Init()
{
	// メニューSpritesの初期化
	menuUIs_ = std::make_unique<MenuUIs>();
	menuUIs_->Init();

	openMenu_ = false;
	cursorIndex_ = -1;
}

void MenuSelector::OpenMenu()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	// ボタンを押したら
	if (input->IsTriggerKey(DIK_ESCAPE) && !openMenu_) {
		openMenu_ = true;
		cursorIndex_ = -1;
	}else if (input->IsTriggerKey(DIK_ESCAPE) && openMenu_) {
		openMenu_ = false;
	}
}

void MenuSelector::Update()
{
	// 開いていなければreturn
	OpenMenu();
	menuUIs_->EnableDraw(openMenu_);
	if (!openMenu_) { return; }

	// Inputを取得
	Input* input = Input::GetInstance();

	// 決定ボタンが押されたら
	if (input->IsTriggerKey(DIK_SPACE) || input->IsTriggerKey(DIK_RETURN)) {
		ChengeScene();
		return;
	}

	// ボタンを押したらIndexを変える
	if (input->IsTriggerKey(DIK_W) || input->IsTriggerKey(DIK_UP)) {
		Reset();
		--cursorIndex_;
		// 範囲内に収める
		cursorIndex_ = std::clamp(cursorIndex_, 0, 3);
	}else if (input->IsTriggerKey(DIK_S) || input->IsTriggerKey(DIK_DOWN)) {
		Reset();
		++cursorIndex_;
		// 範囲内に収める
		cursorIndex_ = std::clamp(cursorIndex_, 0, 3);
	}
	// 0より大きければ
	if (cursorIndex_ > -1) {
		// index番号に応じて点滅させる
		menuUIs_->BlinkingIndex(cursorIndex_);
	}
}

void MenuSelector::ChengeScene()
{
	MenuButtonType menuType = menuUIs_->GetTypeIndex(cursorIndex_);
	switch (menuType)
	{
	case MenuButtonType::Select:
		sceneType_ = SceneType::STAGE_SELECT;
		break;
	case MenuButtonType::Reset:
		break;
	case MenuButtonType::Operation:
		break;
	case MenuButtonType::Back:
		break;
	default:
		break;
	}
}

void MenuSelector::Debug_Gui()
{
}

void MenuSelector::Reset()
{
	if (cursorIndex_ > -1 && cursorIndex_ < 4) { menuUIs_->ResetIndex(cursorIndex_); }
}

