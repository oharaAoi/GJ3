#include "BlockNormal.h"

void BlockNormal::Init() {
	SetName("BlockNormal");
	sprite_ = Engine::GetCanvas2d()->AddSprite("normalBlock.png", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::NormalBlock;
}

void BlockNormal::Update() {
}

void BlockNormal::Debug_Gui() {
}
