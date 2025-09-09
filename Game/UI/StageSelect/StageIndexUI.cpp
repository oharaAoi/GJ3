#include "StageIndexUI.h"

#include "Engine.h"

#include "Engine/System/Manager/TextureManager.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

#include "Engine/Lib/Json/JsonItems.h"

#include "Engine/Lib/Math/MyMath.h"

void StageIndexUI::Init(const std::string& _name){
	SetName(_name);

	param_.FromJson(JsonItems::GetData("UI",GetName()));

	for(int i = 0; i < param_.digitNum; ++i){
		digitSprites_.emplace_back(Engine::GetCanvas2d()->AddSprite(param_.textureName_,GetName() + "_" + std::to_string(i),"Sprite_Normal.json"));
		digitSprites_.back()->ReSetTextureSize(param_.tileSpriteSize_);
		digitSprites_.back()->SetDrawRange(param_.tileTextureSize_);
	}

	EditorWindows::AddObjectWindow(this,GetName());
}

void StageIndexUI::Update(int32_t _targetNum){
	if(digitSprites_.empty()){
		return;
	}

	float lengthAll = param_.tileSpriteSize_.x * float(param_.digitNum) + param_.offset_.x * float(param_.digitNum - 1);
	float startPosX = param_.centerPos_.x + lengthAll * 0.5f;

	// spriteの変更
	for(int32_t i = 0; i < param_.digitNum; ++i){
		int32_t integer = IntegerCount(_targetNum,i + 1);

		Vector2 drawPos = Vector2(startPosX - (param_.tileSpriteSize_.x + param_.offset_.x) * static_cast<float>(i),param_.centerPos_.y + param_.offset_.y * static_cast<float>(i));
		digitSprites_[i]->SetTranslate(drawPos);

		digitSprites_[i]->SetLeftTop({param_.tileTextureSize_.x * integer});
	}
}

void StageIndexUI::Debug_Gui(){
	if(ImGui::InputInt("digitNum",&param_.digitNum)){
		if(digitSprites_.size() >= param_.digitNum){
			digitSprites_.erase(digitSprites_.begin() + param_.digitNum,digitSprites_.end());
		} else if(digitSprites_.size() <= param_.digitNum){
			for(size_t i = digitSprites_.size(); i < param_.digitNum; ++i){
				digitSprites_.emplace_back(Engine::GetCanvas2d()->AddSprite(param_.textureName_,GetName() + "_" + std::to_string(i),"Sprite_Normal.json"));
			}
		}
	}

	TextureManager* textureManager = TextureManager::GetInstance();
	std::string textureName = textureManager->SelectTexture(param_.textureName_);
	if(param_.textureName_ != textureName){
		LoadTexture(textureName);
	}

	ImGui::DragFloat2("CenterPos",&param_.centerPos_.x,1.0f);
	ImGui::DragFloat2("TileSpriteSize",&param_.tileSpriteSize_.x,1.0f);
	ImGui::DragFloat2("TileTextureSize",&param_.tileTextureSize_.x,0.01f,0.0f,0.0f);
	ImGui::DragFloat2("Offset",&param_.offset_.x,1.0f);

	for(auto sprite : digitSprites_){
		sprite->ReSetTextureSize(param_.tileSpriteSize_);
		sprite->SetDrawRange(param_.tileTextureSize_);
	}

	if(ImGui::Button("Save")){
		JsonItems::Save("UI",param_.ToJson(GetName()));
	}
}

void StageIndexUI::LoadTexture(const std::string& _textureName){
	param_.textureName_ = _textureName;
	for(auto& sprite : digitSprites_){
		sprite->ReSetTexture(_textureName);
	}
}

json StageIndexUI::Param::ToJson(const std::string& id) const{
	JsonBuilder builder(id);
	builder.Add("digitNum",digitNum);
	builder.Add("offset",offset_);
	builder.Add("centerPos",centerPos_);
	builder.Add("tileSpriteSize",tileSpriteSize_);
	builder.Add("tileTextureSize",tileTextureSize_);
	builder.Add("textureName",textureName_);

	return builder.Build();
}

void StageIndexUI::Param::FromJson(const json& jsonData){
	fromJson(jsonData,"digitNum",digitNum);
	fromJson(jsonData,"offset",offset_);
	fromJson(jsonData,"centerPos",centerPos_);
	fromJson(jsonData,"tileSpriteSize",tileSpriteSize_);
	fromJson(jsonData,"tileTextureSize",tileTextureSize_);
	fromJson(jsonData,"textureName",textureName_);
}
