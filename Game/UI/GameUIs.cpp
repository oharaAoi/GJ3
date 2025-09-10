#include "GameUIs.h"

#include "Engine.h"
#include "Engine/System/Input/Input.h"

#include "Engine/System/Editer/Window/EditorWindows.h"

bool GameUIs::activeKeyboardUIs_ = true;
bool GameUIs::activeGamePadUIs_ = false;

GameUIs::GameUIs():AttributeGui(){}
GameUIs::~GameUIs(){}

void GameUIs::Init(){
	auto canvas2d = Engine::GetCanvas2d();

	// undo
	undoUIs_[0] = canvas2d->AddSprite("Game_undo_k.png","Undo_Keyboard","Sprite_Normal.json",7,false);
	undoUIs_[1] = canvas2d->AddSprite("Game_undo_p.png","Undo_Pad","Sprite_Normal.json",7,false);
	undoUIs_[2] = canvas2d->AddSprite("Undo.png","Undo_Icon","Sprite_Normal.json",7,false);
	undoUIs_[0]->ApplySaveData();
	undoUIs_[1]->ApplySaveData();
	undoUIs_[2]->ApplySaveData();

	AddChild(undoUIs_[0]);
	AddChild(undoUIs_[1]);
	AddChild(undoUIs_[2]);

	// redo
	redoUIs_[0] = canvas2d->AddSprite("Game_redo_k.png","Redo_Keyboard","Sprite_Normal.json",7,false);
	redoUIs_[1] = canvas2d->AddSprite("Game_redo_p.png","Redo_Pad","Sprite_Normal.json",7,false);
	redoUIs_[2] = canvas2d->AddSprite("Redo.png","Redo_Icon","Sprite_Normal.json",7,false);
	redoUIs_[0]->ApplySaveData();
	redoUIs_[1]->ApplySaveData();
	redoUIs_[2]->ApplySaveData();

	AddChild(redoUIs_[0]);
	AddChild(redoUIs_[1]);
	AddChild(redoUIs_[2]);

	// menu
	menuUIs_[0] = canvas2d->AddSprite("Game_menu_k.png","Menu_Keyboard","Sprite_Normal.json",7,false);
	menuUIs_[1] = canvas2d->AddSprite("Game_menu_p.png","Menu_Pad","Sprite_Normal.json",7,false);
	menuUIs_[0]->ApplySaveData();
	menuUIs_[1]->ApplySaveData();
	AddChild(menuUIs_[0]);
	AddChild(menuUIs_[1]);

	EditorWindows::AddObjectWindow(this,"Game UIs");

	isUndoInput_ = false;
	isRedoInput_ = false;
	isMenuInput_ = false;

	undoMoniPuniTime_ = 0.f;
	redoMoniPuniTime_ = 0.f;
	menuMoniPuniTime_ = 0.f;
}

void GameUIs::Update(bool _isKeyInput,bool _isPadInput){
	if(_isKeyInput || _isPadInput){
		activeGamePadUIs_ = _isPadInput;
		activeKeyboardUIs_ = _isKeyInput;
	}


	if(activeGamePadUIs_){
		undoUIs_[0]->SetEnable(false);
		undoUIs_[1]->SetEnable(true);

		redoUIs_[0]->SetEnable(false);
		redoUIs_[1]->SetEnable(true);

		menuUIs_[0]->SetEnable(false);
		menuUIs_[1]->SetEnable(true);
	} else if(activeKeyboardUIs_){
		undoUIs_[0]->SetEnable(true);
		undoUIs_[1]->SetEnable(false);

		redoUIs_[0]->SetEnable(true);
		redoUIs_[1]->SetEnable(false);

		menuUIs_[0]->SetEnable(true);
		menuUIs_[1]->SetEnable(false);
	}

	/// Undo 
	if(isUndoInput_){
		undoMoniPuniTime_ += GameTimer::DeltaTime() * 5.0f;
		undoMoniPuniTime_ = std::clamp(undoMoniPuniTime_,0.0f,1.0f);
		undoUIs_[0]->SetScale(Vector2::MochiPuniScaleNormalized(undoMoniPuniTime_));
		undoUIs_[1]->SetScale(Vector2::MochiPuniScaleNormalized(undoMoniPuniTime_));
		if(undoMoniPuniTime_ == 1.0f){
			isUndoInput_ = false;
			undoMoniPuniTime_ = 0.f;
		}
	}

	// Redo
	if(isRedoInput_){
		redoMoniPuniTime_ += GameTimer::DeltaTime() * 5.0f;
		redoMoniPuniTime_ = std::clamp(redoMoniPuniTime_,0.0f,1.0f);
		redoUIs_[0]->SetScale(Vector2::MochiPuniScaleNormalized(redoMoniPuniTime_));
		redoUIs_[1]->SetScale(Vector2::MochiPuniScaleNormalized(redoMoniPuniTime_));
		if(redoMoniPuniTime_ == 1.0f){
			isRedoInput_ = false;
			redoMoniPuniTime_ = 0.f;
		}
	}

	// Menu
	if(isMenuInput_){
		menuMoniPuniTime_ += GameTimer::DeltaTime() * 5.0f;
		menuMoniPuniTime_ = std::clamp(menuMoniPuniTime_,0.0f,1.0f);
		menuUIs_[0]->SetScale(Vector2::MochiPuniScaleNormalized(menuMoniPuniTime_));
		menuUIs_[1]->SetScale(Vector2::MochiPuniScaleNormalized(menuMoniPuniTime_));
		if(menuMoniPuniTime_ == 1.0f){
			isMenuInput_ = false;
			menuMoniPuniTime_ = 0.f;
		}
	}

}

void GameUIs::Debug_Gui(){}

void GameUIs::ResetUIInputState(){
	isUndoInput_ = false;
	isRedoInput_ = false;
	isMenuInput_ = false;
}
