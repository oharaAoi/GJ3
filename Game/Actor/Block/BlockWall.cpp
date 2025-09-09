#include "BlockWall.h"

void BlockWall::Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize) {
	SetName("BlockWall");
	sprite_ = _canvas2d->AddSprite("wall.png", "wall", "Sprite_Normal.json",0,true);
	transform_ = sprite_->GetTransform();
	transform_->SetTranslate(_pos);
	tileSize_ = _tileSize;
	type_ = BlockType::Wall;

	sprite_->SetColor({0.56f,0.56f,0.56f,1.f});
}

void BlockWall::Update() {
}

void BlockWall::Debug_Gui() {
}
