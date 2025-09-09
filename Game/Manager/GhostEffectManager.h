#pragma once
#include <list>
#include <memory>
#include "Game/Effect/GhostEffect.h"

class GhostEffectManager :
public AttributeGui {
public:

	GhostEffectManager() = default;
	~GhostEffectManager();
	GhostEffectManager(const GhostEffectManager&) = delete;
	const GhostEffectManager& operator=(const GhostEffectManager&) = delete;

	static GhostEffectManager* GetInstance();

	void Finalize();

	void Init();

	void Update();

	void Draw() const;

	GhostEffect* Create(const Vector2& pos, const Vector2& tileSize);

	void Debug_Gui() override;

private:

	std::list<std::unique_ptr<GhostEffect>> ghostEffect_;

};

