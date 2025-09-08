#include "BlockGoal.h"

void BlockGoal::Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize) {
	SetName("BlockWall");
	sprite_ = _canvas2d->AddSprite("goal.png", "goal", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	transform_->SetTranslate(_pos);
	tileSize_ = _tileSize;
	type_ = BlockType::Goal;
}

void BlockGoal::Update() {
}

void BlockGoal::Debug_Gui() {
}
