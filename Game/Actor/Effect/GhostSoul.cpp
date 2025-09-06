#include "GhostSoul.h"
#include "Engine/Lib/Math/Easing.h"

void GhostSoul::Init(Canvas2d* _canvas2d, const Vector2& _tileSize) {
	SetName("Soul");
	sprite_ = _canvas2d->AddSprite("soul.png", "soul", "Sprite_Normal.json", 100, true);
	sprite_->SetIsFront(true);
	sprite_->ReSetTextureSize(_tileSize);
	transform_ = sprite_->GetTransform();
}

void GhostSoul::Update(const Vector2& _playerPos) {
	Vector2 soulPos = transform_->GetTranslate();
	soulPos = Vector2::Lerp(soulPos, _playerPos, CallEasing(easeType_, easeT_));
	transform_->SetTranslate(soulPos);
}

void GhostSoul::Debug_Gui() {
	SelectEasing(easeType_);
}
