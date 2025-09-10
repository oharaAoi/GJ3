#include "GhostSoul.h"

#include <random>

#include "Engine/Lib/Math/Easing.h"
#include "Engine/Lib/GameTimer.h"

void GhostSoul::Init(Canvas2d* _canvas2d, const Vector2& _tileSize) {
	SetName("Soul");
	pCanvas2d_ = _canvas2d;
	sprite_ = _canvas2d->AddSprite("soul.png", "soul", "Sprite_Normal.json", 100, true);
	// sprite_->ApplySaveData();

	sprite_->ReSetTextureSize(_tileSize);
	transform_ = sprite_->GetTransform();

	easeType_ = (int)EasingType::In::Cubic;

	angleRad_ = 0;

	int rand = std::rand() % 10 + 1;
	swayY_ = kPI2 / static_cast<float>(rand);
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

const float GhostSoul::SwayMoveY()
{
	swayY_ += GameTimer::DeltaTime();
	if (swayY_ > kPI2) { swayY_ = 0.0f; }
	float posY = std::sin(swayY_) * 25.0f;
	return posY;
}
