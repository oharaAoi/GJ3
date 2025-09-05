#include "BlockGrave.h"

void BlockGrave::Init(Canvas2d* _canvas2d)
{
	SetName("Grave");
	sprite_ = _canvas2d->AddSprite("grave.png", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::GraveBlock;
}

void BlockGrave::Update()
{
}

void BlockGrave::Debug_Gui()
{
}
