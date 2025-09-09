#include "BackGround.h"
#include "Engine.h"

void BackGround::Init(Canvas2d* _canvas2d) {
	SetName("backGround");
	sprite_ = _canvas2d->AddSprite("Game_bg.png", "backGround", "Sprite_Normal.json", -10, true);
	sprite_->SetIsBackGround(true);
	sprite_->ApplySaveData();

	sprite_->SetTranslate(Vector2(640.0f, 360.0f));
}

void BackGround::Update() {

}

void BackGround::Debug_Gui() {
	sprite_->Debug_Gui();
}
