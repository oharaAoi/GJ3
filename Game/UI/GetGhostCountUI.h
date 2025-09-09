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
		Vector2 ghostCountTextPos;

		Parameter() { SetName("GetGhostCountUIParam"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("nowPos", nowPos)
				.Add("targetPos", targetPos)
				.Add("barPos", barPos)
				.Add("numberSize", numberSize)
				.Add("numberColor", color)
				.Add("ghostCountTextPos", ghostCountTextPos)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "nowPos", nowPos);
			fromJson(jsonData, "targetPos", targetPos);
			fromJson(jsonData, "barPos", barPos);
			fromJson(jsonData, "numberSize", numberSize);
			fromJson(jsonData, "numberColor", color);
			fromJson(jsonData, "ghostCountTextPos", ghostCountTextPos);
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
	// 今のカウント数
	std::vector<Sprite*> targetCount_;
	// 占いクリスタルの表示
	Sprite* crystalSprite_;
	// おばけのカウントテキスト
	Sprite* ghostCountText_;
	float updownTimer_ = 0.0f;
	// カウントが増えた時のプ二っと演出
	float mochipuniTimer_ = 0.0f;
	// 今のゴーストカウント、前のゴーストカウント
	int nowCount_, backCount_;
	

	Vector2 numberSize_ = { 64.0f, 64.0f };

	Parameter param_;

};

