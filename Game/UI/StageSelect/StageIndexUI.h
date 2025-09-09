#pragma once

#include <vector>
#include <string>

#include "Engine/Module/Components/2d/Sprite.h"

#include "Engine/Module/Components/Attribute/AttributeGui.h"

#include "Engine/Lib/Json/IJsonConverter.h"

class StageIndexUI
	:public AttributeGui{
public:
	StageIndexUI() = default;
	~StageIndexUI() = default;

	void Init(const std::string& _name);
	void Update(int32_t _targetNum);
	void Debug_Gui() override;

	void ApplySaveData();

	void LoadTexture(const std::string& _textureName);
private:
	struct Param
		:public IJsonConverter{
		json ToJson(const std::string& id) const override;
		void FromJson(const json& jsonData) override;

		int32_t digitNum = 2;

		Vector2 offset_ = {0,0};
		Vector2 centerPos_ = {0,0};
		Vector2 tileSpriteSize_ = {0,0};
		Vector2 tileTextureSize_ = {0,0};

		std::string textureName_ = "white.png";
	};
private:
	std::vector<Sprite*> digitSprites_;

	Param param_;
public:
	int32_t GetDigitNum() const{ return param_.digitNum; }

	const Vector2& GetCenterPos() const{ return param_.centerPos_; }
	void SetCenterPos(const Vector2& pos){ param_.centerPos_ = pos; }

	const Vector2& GetTileSpriteSize() const{ return param_.tileSpriteSize_; }
	void SetTileSpriteSize(const Vector2& size){ param_.tileSpriteSize_ = size; }

	const Vector2& GetTileTextureSize() const{ return param_.tileTextureSize_; }
	void SetTileTextureSize(const Vector2& size){ param_.tileTextureSize_ = size; }

	const Vector2& GetOffset() const{ return param_.offset_; }
	void SetOffset(const Vector2& offset){ param_.offset_ = offset; }

	const std::string& GetTextureName() const{ return param_.textureName_; }
};