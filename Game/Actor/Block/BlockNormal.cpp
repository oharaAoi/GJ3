#include "BlockNormal.h"

void BlockNormal::Init(Canvas2d* _canvas2d) {
	SetName("BlockNormal");
	sprite_ = _canvas2d->AddSprite("normalBlock.png", "normalBlock", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	type_ = BlockType::NormalBlock;
}

void BlockNormal::Update() {
	/// 座標更新
	transform_->SetTranslate(ConvertIndexToPosition(index_));
}

void BlockNormal::Debug_Gui() {

	ImGui::Text("X : %d\n Y : %d", index_.x, index_.y);
}

Vector2 BlockNormal::ConvertIndexToPosition(const Vector2Int& _index) {
	return Vector2(offset_.x + _index.x * tileSize_.x + tileSize_.x / 2.0f,
		offset_.y + _index.y * tileSize_.y + tileSize_.y / 2.0f);
}
