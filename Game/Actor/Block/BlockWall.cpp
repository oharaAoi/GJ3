#include "BlockWall.h"

void BlockWall::Init(Canvas2d* _canvas2d) {
	SetName("BlockWall");
	sprite_ = _canvas2d->AddSprite("wall.png", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::Wall;
}

void BlockWall::Update() {
}

void BlockWall::Debug_Gui() {
}
