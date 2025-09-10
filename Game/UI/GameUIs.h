#pragma once

#include "Engine/Module/Components/Attribute/AttributeGui.h"

#include "Engine/Module/Components/2d/Sprite.h"

class PlayerInputHandler;

class GameUIs
	:public AttributeGui{
	static bool activeKeyboardUIs_;
	static bool activeGamePadUIs_;
public:
	GameUIs();
	~GameUIs() override;

	void Init();
	void Update(bool _isKeyInput,bool _isPadInput);
	void Debug_Gui() override;

private:
	// true: gamepad, false: keyboard and mouse
	bool isInputGamePad_ = false; 

	// [0] = Keyboard , [1] = GamePad . [2] = icon
	std::array<Sprite*,3> undoUIs_{nullptr,nullptr};
	std::array<Sprite*,3> redoUIs_{nullptr,nullptr};

	// [0] = Keyboard , [1] = GamePad
	std::array<Sprite*,2> menuUIs_{nullptr,nullptr};
};

