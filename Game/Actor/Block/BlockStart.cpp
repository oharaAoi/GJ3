#include "BlockStart.h"

void BlockStart::Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize) {
	SetName("BlockStart");
	sprite_ = _canvas2d->AddSprite("startBlock.png", "startBlock", "Sprite_Normal.json",0,true);
	transform_ = sprite_->GetTransform();
	transform_->SetTranslate(_pos);
	tileSize_ = _tileSize;
	type_ = BlockType::GraveBlock;
}

void BlockStart::Update() {
}

void BlockStart::Debug_Gui() {
}
