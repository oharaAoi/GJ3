#include "BlockNormal.h"

void BlockNormal::Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize) {
	SetName("BlockNormal");
	sprite_ = _canvas2d->AddSprite("normalBlock.png", "normalBlock", "Sprite_Normal.json",0,true);
	transform_ = sprite_->GetTransform();
	transform_->SetTranslate(_pos);
	tileSize_ = _tileSize;
	type_ = BlockType::NormalBlock;
}

void BlockNormal::Update() 
{
	if (backIndex_.x != index_.x || backIndex_.y != index_.y) {
		scaleTimer_ = 0.0f;
	}
	scaleTimer_ += GameTimer::DeltaTime() * 6.0f;
	scaleTimer_ = std::clamp(scaleTimer_, 0.0f, 1.0f);
	transform_->SetScale(Vector2::MochiPuniScaleNormalized(scaleTimer_));

	/// 座標更新
	transform_->SetTranslate(ConvertIndexToPosition(index_));

	backIndex_ = index_;
}

void BlockNormal::Debug_Gui() {

	ImGui::Text("X : %d\n Y : %d", index_.x, index_.y);
}

Vector2 BlockNormal::ConvertIndexToPosition(const Vector2Int& _index) {
	return Vector2(offset_.x + _index.x * tileSize_.x + tileSize_.x / 2.0f,
		offset_.y + _index.y * tileSize_.y + tileSize_.y / 2.0f);
}
