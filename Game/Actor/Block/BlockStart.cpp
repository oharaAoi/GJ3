#include "BlockStart.h"

void BlockStart::Init(Canvas2d* _canvas2d) {
	SetName("BlockStart");
	sprite_ = _canvas2d->AddSprite("startBlock.png", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::GraveBlock;
}

void BlockStart::Update() {
}

void BlockStart::Debug_Gui() {
}
