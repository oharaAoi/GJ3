#include "BlockLimit.h"

void BlockLimit::Init(Canvas2d* _canvas2d)
{
	SetName("BlockLimit");
	sprite_ = _canvas2d->AddSprite("limitBlock.png", "limitBlock", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::LimitBlock;
}

void BlockLimit::Update()
{
	/// 座標更新
	transform_->SetTranslate(ConvertIndexToPosition(index_));
}

void BlockLimit::Debug_Gui()
{
}

Vector2 BlockLimit::ConvertIndexToPosition(const Vector2Int& _index) {
	return Vector2(offset_.x + _index.x * tileSize_.x + tileSize_.x / 2.0f,
		offset_.y + _index.y * tileSize_.y + tileSize_.y / 2.0f);
}