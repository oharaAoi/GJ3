#pragma once

#include "Engine/Module/Components/Attribute/AttributeGui.h"

#include "Engine/Module/Components/2d/Sprite.h"

class PlayerInputHandler;

class GameUIs
	:public AttributeGui{
	static bool activeKeyboardUIs_;
	static bool activeGamePadUIs_;

	bool GetActiveKeyboardUIs(){ return activeKeyboardUIs_; }
	bool GetActiveGamePadUIs(){ return activeGamePadUIs_; }
public:
	GameUIs();
	~GameUIs() override;

	void Init();
	void Update(bool _isKeyInput,bool _isPadInput);
	void Debug_Gui() override;

	void ResetUIInputState();
private:
	// true: gamepad, false: keyboard and mouse
	bool isInputGamePad_ = false;

	bool isUndoInput_ = false;
	bool isRedoInput_ = false;
	bool isMenuInput_ = false;

	// [0] = Keyboard , [1] = GamePad . [2] = icon
	std::array<Sprite*,3> undoUIs_{nullptr,nullptr};
	std::array<Sprite*,3> redoUIs_{nullptr,nullptr};

	float undoMoniPuniTime_ = 0.f;
	float redoMoniPuniTime_ = 0.f;

	// [0] = Keyboard , [1] = GamePad
	std::array<Sprite*,2> menuUIs_{nullptr,nullptr};

	float menuMoniPuniTime_ = 0.f;

public:
	bool IsInputUndo()const{ return isUndoInput_; };
	void ActiveInputUndo(){ isUndoInput_ = true; };
	bool IsInputRedo()const{ return isRedoInput_; };
	void ActiveInputRedo(){ isRedoInput_ = true; };
	bool IsInputMenu()const{ return isMenuInput_; };
	void ActiveInputMenu(){ isMenuInput_ = true; };

};

