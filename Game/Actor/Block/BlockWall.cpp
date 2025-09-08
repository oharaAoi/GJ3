#include "BlockWall.h"

void BlockWall::Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize) {
	SetName("BlockWall");
	sprite_ = _canvas2d->AddSprite("wall.png", "wall", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	transform_->SetTranslate(_pos);
	tileSize_ = _tileSize;
	type_ = BlockType::Wall;
}

void BlockWall::Update() {
}

void BlockWall::Debug_Gui() {
}
