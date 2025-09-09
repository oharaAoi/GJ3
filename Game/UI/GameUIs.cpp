#include "GameUIs.h"

#include "Engine.h"

#include "Engine/System/Editer/Window/EditorWindows.h"

GameUIs::GameUIs():AttributeGui(){}
GameUIs::~GameUIs(){}

void GameUIs::Init(){
	auto canvas2d = Engine::GetCanvas2d();

	// undo
	undoUIs_[0] = canvas2d->AddSprite("Game_undo_k.png","Undo_Keyboard","Sprite_Normal.json",200,false);
	undoUIs_[1] = canvas2d->AddSprite("Game_undo_p.png","Undo_Pad","Sprite_Normal.json",200,false);
	undoUIs_[0]->ApplySaveData();
	undoUIs_[1]->ApplySaveData();
	AddChild(undoUIs_[0]);
	AddChild(undoUIs_[1]);

	// redo
	redoUIs_[0] = canvas2d->AddSprite("Game_redo_k.png","Redo_Keyboard","Sprite_Normal.json",200,false);
	redoUIs_[1] = canvas2d->AddSprite("Game_redo_p.png","Redo_Pad","Sprite_Normal.json",200,false);
	redoUIs_[0]->ApplySaveData();
	redoUIs_[1]->ApplySaveData();
	AddChild(redoUIs_[0]);
	AddChild(redoUIs_[1]);
	
	// reset
	resetUIs_[0] = canvas2d->AddSprite("Game_reset_k.png","Reset_Keyboard","Sprite_Normal.json",200,false);
	resetUIs_[1] = canvas2d->AddSprite("Game_reset_p.png","Reset_Pad","Sprite_Normal.json",200,false);
	resetUIs_[0]->ApplySaveData();
	resetUIs_[1]->ApplySaveData();
	AddChild(resetUIs_[0]);
	AddChild(resetUIs_[1]);

	// menu
	menuUIs_[0] = canvas2d->AddSprite("Game_menu_k.png","Menu_Keyboard","Sprite_Normal.json",200,false);
	menuUIs_[1] = canvas2d->AddSprite("Game_menu_p.png","Menu_Pad","Sprite_Normal.json",200,false);
	menuUIs_[0]->ApplySaveData();
	menuUIs_[1]->ApplySaveData();
	AddChild(menuUIs_[0]);
	AddChild(menuUIs_[1]);
}

void GameUIs::Update(bool _isKeyInput,bool _isPadInput){
	if(_isPadInput){

		return;
	}
	if(_isKeyInput){

		return;
	}


}

void GameUIs::Debug_Gui(){}
