#include "BlockGoal.h"

void BlockGoal::Init() {
	SetName("BlockWall");
	sprite_ = Engine::GetCanvas2d()->AddSprite("goal.png", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::Goal;
}

void BlockGoal::Update() {
}

void BlockGoal::Debug_Gui() {
}
