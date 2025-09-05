#include "IMenuButtonUI.h"

#include "Engine/Lib/GameTimer.h"

void IMenuButtonUI::Blinking()
{
	// フレームを進める
	blinkingFrame_ += GameTimer::DeltaTime();
	if (blinkingFrame_ > blinkingTime_) {
		blinkingFrame_ = 0.0f;
	}
	// カラーをセットする
	float a = blinkingFrame_ / blinkingTime_;
	buttonUI_->SetColor(Vector4{ 1.0f,1.0f,1.0f,a });
}

void IMenuButtonUI::Reset()
{
	blinkingFrame_ = 0.0f;
	buttonUI_->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
}