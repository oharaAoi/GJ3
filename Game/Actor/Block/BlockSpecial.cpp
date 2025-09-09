#include "BlockSpecial.h"

void BlockSpecial::Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize)
{
	SetName("BlockSpecial");
	sprite_ = _canvas2d->AddSprite("specialBlock.png", "specialBlock", "Sprite_Normal.json",0,true);
	transform_ = sprite_->GetTransform();
	transform_->SetTranslate(_pos);
	tileSize_ = _tileSize;
	type_ = BlockType::SpecialBlock;
	isSpecialBlock_ = true;
}

void BlockSpecial::Update()
{
	/// 座標更新
	transform_->SetTranslate(ConvertIndexToPosition(index_));
}

void BlockSpecial::Debug_Gui()
{
}

Vector2 BlockSpecial::ConvertIndexToPosition(const Vector2Int& _index) {
	return Vector2(offset_.x + _index.x * tileSize_.x + tileSize_.x / 2.0f,
		offset_.y + _index.y * tileSize_.y + tileSize_.y / 2.0f);
}