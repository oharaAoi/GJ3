#include "BlockGrave.h"
#include "Engine/System/Manager/ParticleManager.h"

void BlockGrave::Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize)
{
	SetName("Grave");
	sprite_ = _canvas2d->AddSprite("grave.png", "grave", "Sprite_Normal.json");
	transform_ = sprite_->GetTransform();
	tileSize_ = _tileSize;
	type_ = BlockType::GraveBlock;

	timer_ = 0;
	fallTime_ = 0.3f;
	startOffsetPosY_ = 64.0f;
	targetFallPosY_ = _pos.y;

	Vector2 offsetPos = _pos;
	offsetPos.y -= startOffsetPosY_;
	startFallPosY_ = offsetPos.y;
	transform_->SetTranslate(offsetPos);

	smoke_ = ParticleManager::GetInstance()->CrateParticle("testSmoke");
}

void BlockGrave::Update()
{
	FallAppearance();
}

void BlockGrave::Debug_Gui()
{
}

void BlockGrave::FallAppearance() {
	if (timer_ < fallTime_) {
		timer_ += GameTimer::DeltaTime();
		float t = timer_ / fallTime_;

		// 線形補完
		float alpha = std::lerp(0.f, 1.f, t);
		float y = std::lerp(startFallPosY_, targetFallPosY_, t);

		sprite_->SetAlpha(alpha);
		sprite_->SetTranslateY(y);

		if (timer_ >= fallTime_) {
			smoke_->Reset();
		}
	}
}
