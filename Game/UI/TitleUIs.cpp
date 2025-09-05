#include "TitleUIs.h"

/// engine
#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Engine/Lib/Json/JsonItems.h"

void TitleUIs::Init(){
	SetName("TitleUIs");

	///===========================================
	// Sprites
	///===========================================
	backGround_ = Engine::GetCanvas2d()->AddSprite("title_bg.png", "title_bg.png", "Sprite_Normal.json",-10);
	titleBar_ = Engine::GetCanvas2d()->AddSprite("title_bar.png", "title_bar.png", "Sprite_Normal.json",1);

	shelf_ = Engine::GetCanvas2d()->AddSprite("shelf.png", "shelf.png", "Sprite_Normal.json",2);

	ghost_ = Engine::GetCanvas2d()->AddSprite("title_ghost.png", "title_ghost.png", "Sprite_Normal.json",5);


	///===========================================
	// AttributeGui に登録
	///===========================================
	AddChild(titleBar_);
	AddChild(shelf_);
	AddChild(ghost_);
	AddChild(backGround_);

	titleBar_->SetScale({0.8f,0.8f});
	titleBar_->SetTranslate({640.f,242.f});

	shelf_->SetAnchorPoint({1.f,1.f});
	ghost_->SetAnchorPoint({1.f,1.f});
	titleBar_->SetAnchorPoint({0.5f,0.5f});
	backGround_->SetAnchorPoint({1.f,1.f});

	Vector4 defaultColor = {1.f,1.f,1.f,1.f};
	backGround_->SetColor(defaultColor);
	shelf_->SetColor(defaultColor);
	ghost_->SetColor(defaultColor);

	EditorWindows::AddObjectWindow(this,GetName());
}

void TitleUIs::Update(){}

void TitleUIs::Debug_Gui(){}
