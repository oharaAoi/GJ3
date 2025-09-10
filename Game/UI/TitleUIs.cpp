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
	backGround_ = Engine::GetCanvas2d()->AddSprite("title_bg.png","BackGround","Sprite_Normal.json",-10,true);
	backGround_->SetColor({0.15f,0.15f,0.15f,1.f});

	titleWords_[0] = Engine::GetCanvas2d()->AddSprite("Title_su.png","su","Sprite_None.json",-1);   // す
	titleWords_[1] = Engine::GetCanvas2d()->AddSprite("Title_ki.png","ki","Sprite_None.json",-1);   // き
	titleWords_[2] = Engine::GetCanvas2d()->AddSprite("Title_da.png","da","Sprite_None.json",-1);   // だ
	titleWords_[3] = Engine::GetCanvas2d()->AddSprite("Title_ma.png","ma","Sprite_None.json",-1);   // ま
	titleWords_[4] = Engine::GetCanvas2d()->AddSprite("Title_ri.png","ri","Sprite_None.json",-1);   // り

	for(auto word : titleWords_){
		word->SetColor({1.f,1.f,1.f,1.f});
	}

	spaceKey_ = Engine::GetCanvas2d()->AddSprite("Title_space.png","SpaceKey","Sprite_Normal.json",6);

	shelf_ = Engine::GetCanvas2d()->AddSprite("Title_shelf.png","Shelf","Sprite_Normal.json",-1,true);
	shelf_->SetColor({0.15f,0.15f,0.15f,1.f});

	ghost_ = Engine::GetCanvas2d()->AddSprite("title_ghost.png","Ghost","Sprite_Normal.json",5);

	//param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));

	///===========================================
	// AttributeGui に登録
	///===========================================
	for (size_t i = 0; i < titleWords_.size(); ++i) {
		titleWords_[i]->ApplySaveData();
		AddChild(titleWords_[i]);
	}
	titleWords_[0]->SetAnchorPoint({ 0.9f,0.1f });
	titleWords_[1]->SetAnchorPoint({ 0.5f,0.1f });
	titleWords_[2]->SetAnchorPoint({ 0.9f,0.1f });
	titleWords_[3]->SetAnchorPoint({ 0.1f,0.1f });
	titleWords_[4]->SetAnchorPoint({ 0.5f,0.1f });
	AddChild(spaceKey_);
	AddChild(shelf_);
	AddChild(ghost_);
	AddChild(backGround_);

	spaceKey_->SetAnchorPoint({1.f,1.f});
	shelf_->SetAnchorPoint({1.f,1.f});
	ghost_->SetAnchorPoint({1.f,1.f});
	backGround_->SetAnchorPoint({1.f,1.f});

	EditorWindows::AddObjectWindow(this,GetName());
}

void TitleUIs::Update(){}

void TitleUIs::Debug_Gui(){}
