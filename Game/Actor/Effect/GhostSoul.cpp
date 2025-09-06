#include "GhostSoul.h"
#include "Engine/Lib/Math/Easing.h"
#include "Engine/Lib/GameTimer.h"

void GhostSoul::Init(Canvas2d* _canvas2d, const Vector2& _tileSize) {
	SetName("Soul");
	sprite_ = _canvas2d->AddSprite("soul.png", "soul", "Sprite_Normal.json", 100, true);
	sprite_->SetIsFront(true);
	sprite_->ReSetTextureSize(_tileSize);
	transform_ = sprite_->GetTransform();

	easeType_ = (int)EasingType::In::Cubic;

	angleRad_ = 0;
}

void GhostSoul::Update(const Vector2& _preIndexPos) {
	Vector2 soulPos = transform_->GetTranslate();
	Vector2 newPos = Vector2::Lerp(soulPos, _preIndexPos, CallEasing(easeType_, easeT_));

	transform_->SetTranslate(newPos);
}

void GhostSoul::Debug_Gui() {
	SelectEasing(easeType_);
	ImGui::DragFloat("eaasT", &easeT_, 0.1f);
}

void GhostSoul::RotatePlayer(const Vector2& _playerPos) {
	Vector2 soulPos = transform_->GetTranslate();

	angleRad_ += angleSpeed_ * GameTimer::DeltaTime();
	float cos = std::cosf(angleRad_);
	float sin = std::sinf(angleRad_);

	Vector2 distance = soulPos - _playerPos;
	soulPos.x += cos - sin;
	soulPos.y += sin + cos;
	transform_->SetTranslate(soulPos);
}
