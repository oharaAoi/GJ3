#include "BlockWall.h"

void BlockWall::Init() {
	SetName("BlockWall");
	sprite_ = Engine::GetCanvas2d()->AddSprite("wall.png", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::Wall;
}

void BlockWall::Update() {
}

void BlockWall::Debug_Gui() {
}
