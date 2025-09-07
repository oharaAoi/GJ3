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
	backGround_ = Engine::GetCanvas2d()->AddSprite("title_bg.png","BackGround","Sprite_Normal.json",-10);

	titleWords_[0] = Engine::GetCanvas2d()->AddSprite("Title_su.png","su","Sprite_Normal.json",2);   // す
	titleWords_[1] = Engine::GetCanvas2d()->AddSprite("Title_ki.png","ki","Sprite_Normal.json",2);   // き
	titleWords_[2] = Engine::GetCanvas2d()->AddSprite("Title_da.png","da","Sprite_Normal.json",2);   // だ
	titleWords_[3] = Engine::GetCanvas2d()->AddSprite("Title_ma.png","ma","Sprite_Normal.json",2);   // ま
	titleWords_[4] = Engine::GetCanvas2d()->AddSprite("Title_ri.png","ri","Sprite_Normal.json",2);   // り

	spaceKey_ = Engine::GetCanvas2d()->AddSprite("Title_space.png","SpaceKey","Sprite_Normal.json",0);

	shelf_ = Engine::GetCanvas2d()->AddSprite("Title_shelf.png","Shelf","Sprite_Normal.json",3);

	ghost_ = Engine::GetCanvas2d()->AddSprite("title_ghost.png","Ghost","Sprite_Normal.json",1);

	///===========================================
	// AttributeGui に登録
	///===========================================
	for(auto& wordSprite : titleWords_){
		AddChild(wordSprite);
		wordSprite->SetAnchorPoint({1.f,1.f});
	}
	AddChild(spaceKey_);
	AddChild(shelf_);
	AddChild(ghost_);
	AddChild(backGround_);

	spaceKey_->SetAnchorPoint({1.f,1.f});
	shelf_->SetAnchorPoint({1.f,1.f});
	ghost_->SetAnchorPoint({1.f,1.f});
	backGround_->SetAnchorPoint({1.f,1.f});

	Vector4 defaultColor = {1.f,1.f,1.f,1.f};
	backGround_->SetColor(defaultColor);
	shelf_->SetColor(defaultColor);
	ghost_->SetColor(defaultColor);

	EditorWindows::AddObjectWindow(this,GetName());
}

void TitleUIs::Update(){}

void TitleUIs::Debug_Gui(){}
