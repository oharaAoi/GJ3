#pragma once
#include <vector>
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/Lib/Json/IJsonConverter.h"

/// <summary>
/// Ghostの取得数を表示する関数
/// </summary>
class GetGhostCountUI :
	public AttributeGui {
public:

	struct Parameter : public IJsonConverter {
		Vector2 nowPos;
		Vector2 targetPos;
		Vector2 barPos;
		Vector2 numberSize;
		Vector4 color;

		Parameter() { SetName("GetGhostCountUIParam"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("nowPos", nowPos)
				.Add("targetPos", targetPos)
				.Add("barPos", barPos)
				.Add("numberSize", numberSize)
				.Add("numberColor", color)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "nowPos", nowPos);
			fromJson(jsonData, "targetPos", targetPos);
			fromJson(jsonData, "barPos", barPos);
			fromJson(jsonData, "numberSize", numberSize);
			fromJson(jsonData, "numberColor", color);
		}
	};

public:

	GetGhostCountUI() = default;
	~GetGhostCountUI() = default;

	void Init(Canvas2d* _canvas2d);

	void Update(uint32_t _nowCount, uint32_t targetCount);

	void Debug_Gui() override;

private:

	Canvas2d* pCanvas2d_;

	std::vector<Sprite*> targetCount_;

	Sprite* crystalSprite_;

	Vector2 numberSize_ = { 64.0f, 64.0f };

	Parameter param_;

};

