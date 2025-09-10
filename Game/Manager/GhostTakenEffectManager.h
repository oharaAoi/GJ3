#pragma once
#include <list>
#include <memory>
#include "Game/Effect/GhostTakenEffect.h"

/// <summary>
/// ghostの取得したエフェクトを管理のマネージャー
/// </summary>
class GhostTakenEffectManager :
	public AttributeGui {
public:

	GhostTakenEffectManager() = default;
	~GhostTakenEffectManager();
	GhostTakenEffectManager(const GhostTakenEffectManager&) = delete;
	const GhostTakenEffectManager& operator=(const GhostTakenEffectManager&) = delete;

	static GhostTakenEffectManager* GetInstance();

	void Finalize();

	void Init();

	void Update();

	void Draw() const;

	void Create(const Vector2& pos, const Vector2& tileSize);

	void Debug_Gui() override;

private:

	std::list<std::unique_ptr<GhostTakenEffect>> ghostTakenEffect_;

};

