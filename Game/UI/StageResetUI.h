#pragma once

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/Lib/Json/IJsonConverter.h"

class StageResetUI:
	public AttributeGui
{
public:

	struct Parameter : public IJsonConverter {
		Vector2 resetButtonPos;
		Vector2 backTexturePos;
		float def_size;
		float resetTime;

		Parameter() { SetName("StageResetUIParam"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("resetButtonPos", resetButtonPos)
				.Add("backTexturePos", backTexturePos)
				.Add("def_size", def_size)
				.Add("resetTime", resetTime)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "resetButtonPos", resetButtonPos);
			fromJson(jsonData, "backTexturePos", backTexturePos);
			fromJson(jsonData, "def_size", def_size);
			fromJson(jsonData, "resetTime", resetTime);
		}
	};


public:

	StageResetUI() = default;
	~StageResetUI() = default;

	void Init(Canvas2d* _canvas2d);

	void Update();

	void Debug_Gui() override;
	
	void SetTextureSize(const Vector2& size) { textureSize_ = size; }

	bool GetStageReset()const { return isStageReset_; }
	void Reset() {
		resetTimer_ = 0.0f;
		isStageReset_ = false;
	}

private:

	Sprite* resetButtonUI_ = nullptr;

	Sprite* backTextureUI_ = nullptr;
	Sprite* whiteTextureUI_ = nullptr;

	Vector2 textureSize_ = {};
	bool isStageReset_ = false;
	float resetTimer_ = 0.0f;
	float kResetTime_ = 1.0f;

	Canvas2d* pCanvas2d_;

	Parameter param_;
};

