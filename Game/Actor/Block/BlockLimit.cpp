#include "BlockLimit.h"
#include "Game/Manager/LimitBlockEffectManager.h"

BlockLimit::~BlockLimit() {
	if (effect_) {
		effect_->SetIsDestroy(true);
	}
}

void BlockLimit::Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize)
{
	SetName("BlockLimit");
	sprite_ = _canvas2d->AddSprite("limitBlock.png", "limitBlock", "Sprite_Normal.json",0,true);
	transform_ = sprite_->GetTransform();
	transform_->SetTranslate(_pos);
	tileSize_ = _tileSize;
	type_ = BlockType::LimitBlock;

	effect_ = LimitBlockEffectManager::GetInstance()->Create(transform_->GetTranslate(), tileSize_ * 3.f);
	effect_->ApplySaveData("LimitBlock");
}

void BlockLimit::Update()
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

void BlockLimit::Debug_Gui()
{
}

Vector2 BlockLimit::ConvertIndexToPosition(const Vector2Int& _index) {
	return Vector2(offset_.x + _index.x * tileSize_.x + tileSize_.x / 2.0f,
		offset_.y + _index.y * tileSize_.y + tileSize_.y / 2.0f);
}