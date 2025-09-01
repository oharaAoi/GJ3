#include "BlockWall.h"

void BlockWall::Init() {
	SetName("TitleUIs");
	sprite_ = Engine::GetCanvas2d()->AddSprite("wall.png", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
}

void BlockWall::Update() {
}

void BlockWall::Debug_Gui() {
}
