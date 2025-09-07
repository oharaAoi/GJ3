#include "BlockGoal.h"

void BlockGoal::Init(Canvas2d* _canvas2d) {
	SetName("BlockWall");
	sprite_ = _canvas2d->AddSprite("goal.png", "goal", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::Goal;
}

void BlockGoal::Update() {
}

void BlockGoal::Debug_Gui() {
}
