#include "BlockGrave.h"

void BlockGrave::Init()
{
	SetName("Grave");
	sprite_ = Engine::GetCanvas2d()->AddSprite("grave.png", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::Grave;
}

void BlockGrave::Update()
{
}

void BlockGrave::Debug_Gui()
{
}
