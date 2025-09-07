#pragma once
#include <memory>
#include <array>

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

#include "Game/UI/Button/IButtonUI.h"
#include "Game/UI/Menu/OperationUI.h"

class MenuUIs : 
	public AttributeGui
{
public:

	MenuUIs() = default;
	~MenuUIs() = default;

	void Init();

	void Update();

	void Debug_Gui() override;

	void FadeIn() { isFade_ = true; endFade_ = false; }
	void FadeOut() { isFade_ = false; endFade_ = false; }

	bool GetEndFade()const { return endFade_; }
	bool GetIsFade()const { return isFade_; }

	ButtonType GetTypeIndex(int index)const { return buttonUIs_[index]->GetButtonType(); }
	void BlinkingIndex(int index) { buttonUIs_[index]->Blinking(); }
	void ResetIndex(int index) { buttonUIs_[index]->Reset(); }
	void OperationUpdate(bool openOperation) { operationUI_->Update(openOperation); }

	// ステージリセット時
	void ResetUIs();

private:

	// メニュー背景
	Sprite* menu_ = nullptr;

	// 操作方法
	std::unique_ptr<OperationUI> operationUI_ = nullptr;

	// ボタン
	std::array<std::unique_ptr<IButtonUI>, 4> buttonUIs_;

	// trueがin、falseがout
	bool isFade_ = false;
	bool endFade_ = false;
	float fadeTime_ = 1.0f;
	float fadeFrame_ = 0.0f;

};

