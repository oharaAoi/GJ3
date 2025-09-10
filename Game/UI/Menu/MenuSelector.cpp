#include "MenuSelector.h"

#include "Engine/System/Input/Input.h"
#include "Engine/System/Audio/AudioPlayer.h"
#include "Engine/Lib/GameTimer.h"

#include "Engine/Module/Components/2d/BaseEntity2d.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

void MenuSelector::Init(Canvas2d* canvas2d)
{
	// メニューSpritesの初期化
	menuUIs_ = std::make_unique<MenuUIs>();
	menuUIs_->Init();

	pCanvas2d_ = canvas2d;

	openMenu_ = false;
	cursorIndex_ = -1;

	Update();
}

void MenuSelector::OpenMenu()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	isKeyInput_ = input->IsTriggerKey(DIK_ESCAPE);
	isPadInput_ = input->IsTriggerButton(XInputButtons::START);

	// ボタンを押したら
	if ((isKeyInput_ || isPadInput_) && !openMenu_) {
		menuUIs_->FadeIn();
		openMenu_ = true;
		openTrigger_ = true;
		cursorIndex_ = -1;
	} else if ((isKeyInput_ || isPadInput_) && openMenu_ &&
		!chengeScene_ && !drawEffect_ && !openOperation_) {
		menuUIs_->FadeOut();
		chengeScene_ = true;
		cursorIndex_ = 3;
	}
}

void MenuSelector::Update()
{
	openTrigger_ = false;

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
		effectFrame_ = std::clamp(effectFrame_, 0.0f, 5.0f);
		menuUIs_->SetColor(cursorIndex_);
		menuUIs_->SetColors(cursorIndex_);
		if (effectFrame_ == 0.0f) {
			chengeScene_ = true;
			drawEffect_ = false;
			menuUIs_->ResetUIs();
		}
		return;
	}
	// ======================================================================
	//								入力処理
	// ======================================================================
	
	const float dt = GameTimer::DeltaTime();
	const auto lsY = input->GetLeftJoyStick(0.2f).y;

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
	if (openOperation_) {
		menuUIs_->SetColor(cursorIndex_);
		menuUIs_->SetColors(cursorIndex_);
	}
	if (decisionPressed && openOperation_) {
		AudioPlayer::SinglShotPlay("button.mp3", 0.3f);
		openOperation_ = false;
		return;
	}
	if (menuUIs_->GetOpEndFade()) {
		SetMenuRenderQueue(10);
	} else {
		menuUIs_->SetColor(cursorIndex_);
		return;
	}
	// メニューを開いていなければreturn
	if (!openMenu_ || chengeScene_ || drawEffect_ || openOperation_) { return; }

	// 決定ボタンが押されたら
	if (decisionPressed) {
		ChengeScene();
		if (cursorIndex_ != -1) {
			menuUIs_->SetColor(cursorIndex_);
			menuUIs_->SetColors(cursorIndex_);
			AudioPlayer::SinglShotPlay("button.mp3", 0.3f);
		}
		return;
	}
	// 上方向の処理
	if (ProcessRepeat(upPressed, upWasPressed_, upHoldTime_, dt)) {
		Reset();
		--cursorIndex_;
		cursorIndex_ = std::clamp(cursorIndex_, 0, 3);
		AudioPlayer::SinglShotPlay("button.mp3", 0.3f);
	}
	// 下方向の処理
	if (ProcessRepeat(downPressed, downWasPressed_, downHoldTime_, dt)) {
		Reset();
		++cursorIndex_;
		cursorIndex_ = std::clamp(cursorIndex_, 0, 3);
		AudioPlayer::SinglShotPlay("button.mp3", 0.3f);
	}
	// 0より大きければ
	if (cursorIndex_ > -1 && !chengeScene_) {
		// index番号に応じて点滅させる
		menuUIs_->BlinkingIndex(cursorIndex_);
		menuUIs_->SetColors(cursorIndex_);
	}
}

void MenuSelector::ChengeScene()
{
	if (cursorIndex_ < 0) { return; }

	const auto type = GetButtonType();

	switch (type)
	{
	case ButtonType::Select:
		chengeScene_ = false;
		drawEffect_ = true;
		changeEffect_ = true;
		effectFrame_ = 2.0f;
		break;
	case ButtonType::Reset:
		chengeScene_ = false;
		drawEffect_ = true;
		changeEffect_ = true;
		effectFrame_ = 4.0f;
		break;
	case ButtonType::Operation:
		openOperation_ = true;
		menuUIs_->SetColor(cursorIndex_);
		SetMenuRenderQueue(15);
		break;
	case ButtonType::Back:
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

void MenuSelector::SetMenuRenderQueue(int renderQueue)
{
	Canvas2d::ObjectPair* pair = pCanvas2d_->GetObjectPair(menuUIs_->GetMenuBG());
	pair->renderQueue = renderQueue;
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
