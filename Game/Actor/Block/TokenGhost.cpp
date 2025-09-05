#include "TokenGhost.h"

void TokenGhost::Init(Canvas2d* _canvas2d)
{
	SetName("TokenGhost");
	sprite_ = _canvas2d->AddSprite("ghost.png", "ghost", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::Ghost;
}

void TokenGhost::Update()
{
	/// 座標更新
	transform_->SetTranslate(ConvertIndexToPosition(index_));
}

void TokenGhost::Debug_Gui()
{
	ImGui::Text("X : %d\n Y : %d", index_.x, index_.y);
}

Vector2 TokenGhost::ConvertIndexToPosition(const Vector2Int& _index) {
	return Vector2(offset_.x + _index.x * tileSize_.x + tileSize_.x / 2.0f,
		offset_.y + _index.y * tileSize_.y + tileSize_.y / 2.0f);
}