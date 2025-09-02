#include "Player.h"

/// hadler
#include "PlayerInputHandler.h"

Player::Player():IBlock(){}
Player::~Player(){}

void Player::Init(){
	/// Tag
	SetName("Player");
	type_ = BlockType::Player;

	/// sprite 
	sprite_ = Engine::GetCanvas2d()->AddSprite("wall.png","Sprite_Normal.json");
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

Vector2 ConvertIndexToPosition(const Vector2Int& _index){
	constexpr Vector2 kBlockSize = {32.0f,32.0f};
	return Vector2(_index.x * kBlockSize.x,_index.y * kBlockSize.y);
}
