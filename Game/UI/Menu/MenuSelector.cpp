#include "MenuSelector.h"

#include "Engine/System/Input/Input.h"
#include "Engine/Lib/GameTimer.h"

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
	if ((input->IsTriggerKey(DIK_ESCAPE) ||
		input->IsTriggerButton(XInputButtons::START)) && !openMenu_) {
		menuUIs_->FadeIn();
		openMenu_ = true;
		cursorIndex_ = -1;
	} else if ((input->IsTriggerKey(DIK_ESCAPE) || input->IsTriggerButton(XInputButtons::START)) && openMenu_ &&
		!chengeScene_ && !drawEffect_) {
		menuUIs_->FadeOut();
	}
}

void MenuSelector::Update()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	// メニューを開く
	OpenMenu();
	// メニューUIの更新
	menuUIs_->Update();
	// メニューが閉じ切った時
	if (menuUIs_->GetEndFade() && !menuUIs_->GetIsFade()) { 
		openMenu_ = false;
		chengeScene_ = false;
	}
	// エフェクトを出している時間
	if (drawEffect_) {
		effectFrame_ -= GameTimer::DeltaTime();
		effectFrame_ = std::clamp(effectFrame_, 0.0f, 1.0f);
		if (effectFrame_ == 0.0f) {
			chengeScene_ = true;
			drawEffect_ = false;
			menuUIs_->ResetUIs();
		}
	}
	// ======================================================================
	//								入力処理
	// ======================================================================
	
	const float dt = GameTimer::DeltaTime();
	const auto lsY = input->GetLeftJoyStick(0.2f).y; // DEAD 0.2

	// 決定が押されているか
	const bool decisionPressed = input->IsTriggerKey(DIK_SPACE) || input->IsTriggerKey(DIK_RETURN) ||
		input->IsTriggerButton(XInputButtons::BUTTON_A) || input->IsTriggerButton(XInputButtons::BUTTON_B);
	// 上入力が押されているか
	const bool upPressed = input->IsPressKey(DIK_W) || input->IsPressKey(DIK_UP) ||
		input->IsPressButton(XInputButtons::DPAD_UP) || (lsY > 0.5f);
	// 下入力が押されているか
	const bool downPressed = input->IsPressKey(DIK_S) || input->IsPressKey(DIK_DOWN) ||
		input->IsPressButton(XInputButtons::DPAD_DOWN) || (lsY < -0.5f);

	// 操作方法を表示する
	menuUIs_->OperationUpdate(openOperation_);
	if (decisionPressed && openOperation_) {
		openOperation_ = false;
		return;
	}
	// メニューを開いていなければreturn
	if (!openMenu_ || chengeScene_ || drawEffect_ || openOperation_) { return; }

	// 決定ボタンが押されたら
	if (decisionPressed) {
		ChengeScene();
		return;
	}
	// 上方向の処理
	if (ProcessRepeat(upPressed, upWasPressed_, upHoldTime_, dt)) {
		Reset();
		--cursorIndex_;
		cursorIndex_ = std::clamp(cursorIndex_, 0, 3);
	}
	// 下方向の処理
	if (ProcessRepeat(downPressed, downWasPressed_, downHoldTime_, dt)) {
		Reset();
		++cursorIndex_;
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
	if (cursorIndex_ < 0) { return; }

	const auto type = GetButtonType();

	switch (type)
	{
	case MenuButtonType::Select:
		chengeScene_ = false;
		drawEffect_ = true;
		effectFrame_ = 1.0f;
		break;
	case MenuButtonType::Reset:
		chengeScene_ = false;
		drawEffect_ = true;
		effectFrame_ = 1.0f;
		break;
	case MenuButtonType::Operation:
		openOperation_ = true;
		break;
	case MenuButtonType::Back:
		menuUIs_->FadeOut();
		chengeScene_ = true;
		break;
	default:
		chengeScene_ = false;
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

bool MenuSelector::ProcessRepeat(bool pressed, bool& wasPressed, float& holdTime, float dt)const
{
	if (!pressed) {
		wasPressed = false;
		holdTime = 0.0f;
		return false;
	}
	if (!wasPressed) {
		wasPressed = true;
		holdTime = 0.0f;
		return true;		// 押された瞬間は即反応
	}
	holdTime += dt;
	const float threshold = (holdTime < repeatDelay_) ? repeatDelay_ : repeatInterval_;
	if (holdTime >= threshold) {
		holdTime = 0.0f;	// 次の間隔計測へ
		return true;
	}
	return false;
}
