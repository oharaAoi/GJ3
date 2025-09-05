#pragma once
#include <memory>

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

#include "Game/Scene/SceneType.h"
#include "Game/UI/Menu/MenuUIs.h"

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

	const SceneType GetSceneType()const { return sceneType_; }
	const bool GetOpenMenu()const { return openMenu_; }
	void Reset();

private:

	// メニュー画面のUI
	std::unique_ptr<MenuUIs> menuUIs_ = nullptr;
	// これを使ってSceneを変える
	SceneType sceneType_ = SceneType::GAME;

	// メニューを開いているか
	bool openMenu_ = false;

	// 今の選択したカーソルIndex
	int cursorIndex_ = -1;

};

