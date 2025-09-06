#pragma once
#include <memory>
#include <vector>
// engine
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"
// game
#include "Game/Actor/Effect/GhostSoul.h"
#include "Game/Actor/Player/Player.h"

/// <summary>
/// Ghostの魂を管理するクラス
/// </summary>
class GhostSoulManager :
	public AttributeGui
{
public:

	GhostSoulManager() = default;
	~GhostSoulManager() = default;

	void Init(Canvas2d* _canvas2d, Player* _player);
	void InitClearScene(Canvas2d* _canvas2d);

	void Update();
	void UpdateClearScene();

	/// <summary>
	/// Soulの生成
	/// </summary>
	void CreateSoul(const Vector2& tileSize);
	void SetPosition(const Vector2& position);

	void Debug_Gui()override;

	struct ClearSoulParam : public IJsonConverter {
		Vector2 center;
		Vector2 radius;
		float def_size;
		float size;

		ClearSoulParam() { SetName("GhostSoulManager"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("center", center)
				.Add("radius", radius)
				.Add("def_size", def_size)
				.Add("size", size)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "center", center);
			fromJson(jsonData, "radius", radius);
			fromJson(jsonData, "def_size", def_size);
			fromJson(jsonData, "size", size);
		}
	};

private:

	Canvas2d* pCanvas2d_;
	Player* pPlayer_;

	// object
	std::vector<std::unique_ptr<GhostSoul>> souls_;

	// 現在角
	float angleRad_ = 0.0f;

	// クリア時ゴーストの調整項目
	ClearSoulParam param_;

};

