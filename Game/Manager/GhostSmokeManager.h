#pragma once
#include <list>
#include <memory>
#include "Game/Effect/GhostSmoke.h"

class GhostSmokeManager :
	public AttributeGui {
public:

	GhostSmokeManager() = default;
	~GhostSmokeManager();
	GhostSmokeManager(const GhostSmokeManager&) = delete;
	const GhostSmokeManager& operator=(const GhostSmokeManager&) = delete;

	static GhostSmokeManager* GetInstance();

	void Finalize();

	void Init();

	void Update();

	void Draw() const;

	std::shared_ptr<GhostSmoke> Create(const Vector2& pos, const Vector2& tileSize);

	void Debug_Gui() override;

private:

	std::list<std::shared_ptr<GhostSmoke>> ghostEffect_;

};

