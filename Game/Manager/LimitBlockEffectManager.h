#pragma once
#include <list>
#include <memory>
#include "Game/Effect/LimitBlockEffect.h"

class LimitBlockEffectManager :
	public AttributeGui {
public:

	LimitBlockEffectManager() = default;
	~LimitBlockEffectManager();
	LimitBlockEffectManager(const LimitBlockEffectManager&) = delete;
	const LimitBlockEffectManager& operator=(const LimitBlockEffectManager&) = delete;

	static LimitBlockEffectManager* GetInstance();

	void Finalize();

	void Init();

	void Update();

	void Draw() const;

	LimitBlockEffect* Create(const Vector2& pos, const Vector2& tileSize);

	void Debug_Gui() override;

private:

	std::list<std::unique_ptr<LimitBlockEffect>> limitBlockEffect_;

};

