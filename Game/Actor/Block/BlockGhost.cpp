#include "BlockGhost.h"

void BlockGhost::Init(Canvas2d* _canvas2d, const Vector2& pos, const Vector2& tileSize)
{
	SetName("BlockGhost");
	sprite_ = _canvas2d->AddSprite("ghostBlock.png", "ghostBlock", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	transform_->SetTranslate(pos);
	tileSize_ = tileSize;
	type_ = BlockType::GhostBlock;
}

void BlockGhost::Update()
{
	/// 座標更新
	transform_->SetTranslate(ConvertIndexToPosition(index_));
}

void BlockGhost::Debug_Gui()
{
}

Vector2 BlockGhost::ConvertIndexToPosition(const Vector2Int& _index) {
	return Vector2(offset_.x + _index.x * tileSize_.x + tileSize_.x / 2.0f,
		offset_.y + _index.y * tileSize_.y + tileSize_.y / 2.0f);
}