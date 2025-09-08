#include "IButtonUI.h"

#include "Engine/Lib/GameTimer.h"

void IButtonUI::Blinking()
{
	// フレームを進める
	blinkingFrame_ += GameTimer::DeltaTime() * 5.0f;
	if (blinkingFrame_ > kPI2) {
		blinkingFrame_ -= kPI2;
	}

	// alphaを0.5～1.0でループさせる
	float alpha = 0.85f + 0.15f * std::cos(blinkingFrame_);
	// カラーをセットする
	buttonUI_->SetColor(Vector4{ 1.0f, 1.0f, 1.0f, alpha });
}

void IButtonUI::Reset()
{
	blinkingFrame_ = 0.0f;
	buttonUI_->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
}