#include "Player.h"

/// handler
#include "Game/Input/PlayerInputHandler.h"

/// info
#include "Game/Information/StageInformation.h"

/// math
#include "Engine/Lib/Math/Easing.h"

Player::Player() : IBlock() {}
Player::~Player() {}

void Player::Init(Canvas2d *_canvas2d, const Vector2& _pos, const Vector2& _tileSize)
{
	/// Tag
	SetName("Player");
	type_ = BlockType::Player;

	/// sprite
	sprite_ = _canvas2d->AddSprite(kPlayerSideTextureName[0], "player", "Sprite_Normal.json", 100, true);
	sprite_->SetIsFront(true);
	transform_ = sprite_->GetTransform();
	transform_->SetTranslate(_pos);
	tileSize_ = _tileSize;

	/// input handler
	inputHandler_ = std::make_unique<PlayerInputHandler>();
	inputHandler_->SetPlayer(this);
}

void Player::Update()
{
	/// 入力処理
	if (inputHandler_)
	{
		inputHandler_->HandleInput();
	}

	/// 移動処理
	Vector2 currentTranslate = transform_->GetTranslate();
	Vector2 targetPos = ConvertIndexToPosition(index_);
	if (isAnimation_)
	{
		/// 補間処理
		animationTimer_ += GameTimer::DeltaTime();

		float t = animationTimer_ / animationTime_;
		t = (std::min)(t, 1.f);

		currentTranslate = Lerp(currentTranslate, targetPos, Ease::InOut::Cubic(t));
		if (t >= 1.f)
		{
			animationTimer_ = 0.f;
			isAnimation_ = false;
		}
	}
	else
	{
		currentTranslate = targetPos;
	}

	transform_->SetTranslate(currentTranslate);

	/// Sprite 切り替え
	this->GetSprite()->SetTextureName(kPlayerSideTextureName[static_cast<int>(spriteDirection_)]);
}

void Player::Debug_Gui()
{
	ImGui::Text("X : %d\n Y : %d", index_.x, index_.y);
}

Vector2 Player::ConvertIndexToPosition(const Vector2Int &_index)
{
	return Vector2(offset_.x + _index.x * tileSize_.x + tileSize_.x / 2.0f,
				   offset_.y + _index.y * tileSize_.y + tileSize_.y / 2.0f);
}
