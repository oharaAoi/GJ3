#pragma once
#include <memory>

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

#include "Game/Scene/SceneType.h"
#include "Game/UI/Menu/MenuUIs.h"
#include "Game/UI/Button/ButtonType.h"

class MenuSelector :
	public AttributeGui
{
public:

	MenuSelector() = default;
	~MenuSelector() = default;

	void Init();

	void OpenMenu();
	void Update();
	void ChengeScene();

	void Debug_Gui()override;

public:

	const ButtonType GetButtonType() const{ return menuUIs_->GetTypeIndex(cursorIndex_); }
	void SetOpenMenu(bool flag) { openMenu_ = flag; }
	const bool GetOpenMenu()const { return openMenu_; }
	const bool GetChengeScene()const { return chengeScene_; }
	void SetChengeScene(bool flag) { chengeScene_ = flag; }

	void Reset();

private:

	bool ProcessRepeat(bool pressed, bool& wasPressed, float& holdTime, float dt)const;

private:

	// メニュー画面のUI
	std::unique_ptr<MenuUIs> menuUIs_ = nullptr;

	// メニューを開いているか
	bool openMenu_ = false;
	// 入力しているか
	float repeatDelay_ = 0.35f;		// 初回リピートまでの遅延秒
	float repeatInterval_ = 0.08f;	// 2回目以降の間隔秒
	bool  upWasPressed_ = false;
	bool  downWasPressed_ = false;
	float upHoldTime_ = 0.0f;
	float downHoldTime_ = 0.0f;
	// シーン切り替えを選択したら
	float effectFrame_ = 0.0f;
	bool drawEffect_ = false;
	bool chengeScene_ = false;
	// 操作方法を開いたら
	bool openOperation_ = false;

	// 今の選択したカーソルIndex
	int cursorIndex_ = -1;

};

