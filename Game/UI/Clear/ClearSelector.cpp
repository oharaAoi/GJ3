#include "ClearSelector.h"

#include "Engine/System/Audio/AudioPlayer.h"
#include "Engine/System/Input/Input.h"
#include "Engine/Lib/GameTimer.h"

#include "Game/UI/StageSelect/StageSelector.h"
#include "Game/Tool/StageLoader.h"

void ClearSelector::Init()
{
	clearUIs_ = std::make_unique<ClearUIs>();
	clearUIs_->Init();

	cursorIndex_ = 1;
}

void ClearSelector::Update()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	clearUIs_->Update();

	// シーン切り替えを選択しているなら
	if (changeScene_ || changeStage_) { 
		changeTimer_ += GameTimer::DeltaTime();
		if (changeTimer_ > 1.0f) {
			changeTimer_ = 0.0f;
			isExecute_ = true;
		}
		return;
	}

	// ======================================================================
	//								入力処理
	// ======================================================================

	const auto lsY = input->GetLeftJoyStick(0.2f).y;

	// 決定が押されているか
	const bool decisionPressed = input->IsTriggerKey(DIK_SPACE) || input->IsTriggerKey(DIK_RETURN) ||
		input->IsTriggerButton(XInputButtons::BUTTON_A) || input->IsTriggerButton(XInputButtons::BUTTON_B);
	// 上入力が押されているか
	const bool upPressed = input->IsTriggerKey(DIK_W) || input->IsTriggerKey(DIK_UP) ||
		input->IsTriggerButton(XInputButtons::DPAD_UP) || (lsY > 0.5f);
	// 下入力が押されているか
	const bool downPressed = input->IsTriggerKey(DIK_S) || input->IsTriggerKey(DIK_DOWN) ||
		input->IsTriggerButton(XInputButtons::DPAD_DOWN) || (lsY < -0.5f);

	if (StageSelector::GetCurrentStageIndex() >= static_cast<int32_t>(StageLoader::GetMaxStageNum() - 1)) {
		// 点滅処理
		clearUIs_->DrawEnable(1);
		cursorIndex_ = 0;
		clearUIs_->BlinkingIndex(cursorIndex_);
		//決定ボタンを押したら
		if (decisionPressed && cursorIndex_ != -1) {
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			const auto type = clearUIs_->GetTypeIndex(cursorIndex_);
			switch (type)
			{
			case ButtonType::Select:
				changeScene_ = true;
				break;
			case ButtonType::NextStage:
				changeStage_ = true;
				break;
			default:
				break;
			}
		}
		return;
	}

	// 入力されていないなら-1で
	cursorIndex_ = std::clamp(cursorIndex_, 0, 1);
	if (cursorIndex_ == -1) {
		if (upPressed) { cursorIndex_ = 0; }
		if (downPressed) { cursorIndex_ = 1; }
	} else {
		if (upPressed) { 
			if (cursorIndex_ == 1) {
				AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			}
			clearUIs_->ResetIndex(cursorIndex_); 
			--cursorIndex_;
		}
		else if (downPressed) { 
			if (cursorIndex_ == 0) {
				AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			}
			clearUIs_->ResetIndex(cursorIndex_); 
			++cursorIndex_; 
		}
		cursorIndex_ = std::clamp(cursorIndex_, 0, 1);
		// ButtonUIの点滅処理
		clearUIs_->BlinkingIndex(cursorIndex_);
		clearUIs_->SetColors(cursorIndex_, 0.2f);
	}
	//決定ボタンを押したら
	if (decisionPressed && cursorIndex_ != -1) {
		AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
		const auto type = clearUIs_->GetTypeIndex(cursorIndex_);
		switch (type)
		{
		case ButtonType::Select:
			changeScene_ = true;
			break;
		case ButtonType::NextStage:
			changeStage_ = true;
			break;
		default:
			break;
		}
	}
}