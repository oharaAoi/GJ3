#include "BackGround.h"
#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

void BackGround::Init() {
	sprite_ = Engine::GetCanvas2d()->AddSprite("backGround.png", "Sprite_Normal.json", -10, true);
	sprite_->SetIsBackGround(true);
	sprite_->SetTranslate(Vector2(640.0f, 360.0f));
}

void BackGround::Update() {

}