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

	void Init(Canvas2d* canvas2d);

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

	const bool GetSelectButton()const { return cursorIndex_ == 0; }
	const bool GetResetButton()const { return cursorIndex_ == 1; }
	const bool GetChangeEffect()const { return changeEffect_; }
	void SetChangeEffect(bool flag) { changeEffect_ = flag; }

	void SetMenuRenderQueue(int renderQueue);

	void Reset();

	bool GetKeyInput()const{ return isKeyInput_; }
	bool GetPadInput()const{ return isPadInput_; }
	bool GetOpenTrigger()const{ return openTrigger_; }
private:

	bool ProcessRepeat(bool pressed, bool& wasPressed, float& holdTime, float dt)const;

private:

	// メニュー画面のUI
	std::unique_ptr<MenuUIs> menuUIs_ = nullptr;

	bool isKeyInput_ = false;
	bool isPadInput_ = false;

	// メニューを開いているか
	bool openTrigger_ = false;
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
	bool changeEffect_ = false;
	bool chengeScene_ = false;
	// 操作方法を開いたら
	bool openOperation_ = false;

	// 今の選択したカーソルIndex
	int cursorIndex_ = -1;

};

