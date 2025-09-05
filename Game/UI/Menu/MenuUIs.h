#pragma once
#include <memory>
#include <array>

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

#include "Game/UI/Menu/button/IMenuButtonUI.h"

class MenuUIs : 
	public AttributeGui
{
public:

	MenuUIs() = default;
	~MenuUIs() = default;

	void Init();

	void Update();

	void Debug_Gui() override;

	void EnableDraw(bool enable);

	MenuButtonType GetTypeIndex(int index)const { return buttonUIs_[index]->GetButtonType(); }
	void BlinkingIndex(int index) { buttonUIs_[index]->Blinking(); }
	void ResetIndex(int index) { buttonUIs_[index]->Reset(); }

private:

	// メニュー背景
	Sprite* menu_ = nullptr;

	// ボタン
	std::array<std::unique_ptr<IMenuButtonUI>, 4> buttonUIs_;

};

