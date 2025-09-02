#include "Player.h"

/// hadler
#include "PlayerInputHandler.h"
#include "Game/Information/StageInformation.h"

Player::Player():IBlock(){}
Player::~Player(){}

void Player::Init(){
	/// Tag
	SetName("Player");
	type_ = BlockType::Player;

	/// sprite 
	sprite_ = Engine::GetCanvas2d()->AddSprite("player.png","Sprite_Normal.json", 100, true);
	sprite_->SetIsFront(true);
	transform_ = sprite_->GetTransform();

	/// input handler
	inputHandler_ = std::make_unique<PlayerInputHandler>();
	inputHandler_->SetPlayer(this);
}

void Player::Update(){
	/// 入力処理
	if(inputHandler_){
		inputHandler_->HandleInput();
	}
	/// 座標更新
	transform_->SetTranslate(ConvertIndexToPosition(index_));
}

void Player::Debug_Gui(){
	ImGui::Text("X : %d\n Y : %d",index_.x,index_.y);
}

Vector2 Player::ConvertIndexToPosition(const Vector2Int& _index) {
	return Vector2(offset_.x + _index.x * tileSize_.x + tileSize_.x / 2.0f,
				   offset_.y + _index.y * tileSize_.y + tileSize_.y / 2.0f);
}
