#include "TitleUIs.h"

/// engine
#include "Engine.h"
#include "Engine/System/Input/Input.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Engine/Lib/Math/Easing.h"

void TitleUIs::Init(){
	SetName("TitleUIs");

	///===========================================
	// Sprites
	///===========================================

	backGround_ = Engine::GetCanvas2d()->AddSprite("title_bg.png","BackGround","Sprite_Normal.json",-10,true);

	titleWords_[0] = Engine::GetCanvas2d()->AddSprite("Title_su.png","su","Sprite_None.json",-1);   // す
	titleWords_[1] = Engine::GetCanvas2d()->AddSprite("Title_ki.png","ki","Sprite_None.json",-1);   // き
	titleWords_[2] = Engine::GetCanvas2d()->AddSprite("Title_da.png","da","Sprite_None.json",-1);   // だ
	titleWords_[3] = Engine::GetCanvas2d()->AddSprite("Title_ma.png","ma","Sprite_None.json",-1);   // ま
	titleWords_[4] = Engine::GetCanvas2d()->AddSprite("Title_ri.png","ri","Sprite_None.json",-1);   // り

	for(auto word : titleWords_){
		word->SetColor({1.f,1.f,1.f,1.f});
	}

	spaceKey_ = Engine::GetCanvas2d()->AddSprite("Title_space.png","SpaceKey","Sprite_Normal.json",6);
	aButton_  = Engine::GetCanvas2d()->AddSprite("Title_A.png","AButton","Sprite_Normal.json",6);

	shelf_ = Engine::GetCanvas2d()->AddSprite("Title_shelf.png","Shelf","Sprite_Normal.json",-1,true);

	ghost_ = Engine::GetCanvas2d()->AddSprite("title_ghost.png","Ghost","Sprite_Normal.json",5);

	//param_.FromJson(JsonItems::GetData(GetName(), param_.GetName()));

	///===========================================
	// AttributeGui に登録
	///===========================================
	for(size_t i = 0; i < titleWords_.size(); ++i){
		titleWords_[i]->ApplySaveData();
		AddChild(titleWords_[i]);
	}
	AddChild(spaceKey_);
	AddChild(shelf_);
	AddChild(ghost_);
	AddChild(backGround_);

	shelf_->ApplySaveData();
	backGround_->ApplySaveData();

	spaceKey_->SetTranslate(Vector2{kWindowWidth_ / 2.0f,kWindowHeight_ / 2.0f});
	shelf_->SetTranslate(Vector2{kWindowWidth_ / 2.0f,kWindowHeight_ / 2.0f});
	ghost_->SetTranslate(Vector2{kWindowWidth_ / 2.0f,kWindowHeight_ / 2.0f});
	backGround_->SetTranslate(Vector2{kWindowWidth_ / 2.0f,kWindowHeight_ / 2.0f});

	EditorWindows::AddObjectWindow(this,GetName());
}

void TitleUIs::Update(){
	if(Input::IsControllerConnected()){
		spaceKey_->SetEnable(false);
		aButton_->SetEnable(true);
	} else{
		spaceKey_->SetEnable(true);
		aButton_->SetEnable(false);
	}

	uiAnimationTimer_ += GameTimer::DeltaTime();
	
	if(uiAnimationTimer_ >= uiAnimationTime_){
		std::swap(startAlpha_,endAlpha_);
		uiAnimationTimer_ = 0.f;
	}

	float t = uiAnimationTimer_ / uiAnimationTime_;

	float alpha = std::lerp(startAlpha_,endAlpha_,Ease::InOut::Cubic(t));
	spaceKey_->SetAlpha(alpha);
	aButton_->SetAlpha(alpha);

}

void TitleUIs::Debug_Gui(){}
