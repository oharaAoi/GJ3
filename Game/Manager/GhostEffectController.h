#pragma once

#include <memory>

#include <unordered_map>

#include "Game/Manager/StageRegistry.h"

#include "Game/Effect/GhostEffect.h"

#include "Engine/Lib/Math/Vector2Int.h"

// ハッシュ関数を定義
struct Vector2IntHash{
	std::size_t operator()(const Vector2Int& v) const noexcept{
		return (std::hash<int>()(v.x) << 1) ^ std::hash<int>()(v.y);
	}
};

class GhostEffectController{
public:
	static GhostEffectController* GetInstance(){
		static GhostEffectController instance;
		return &instance;
	}
public:
	void Init(StageRegistry* _stageRegistry);
	void Update();
	void Finalize();

	void CreateGhostEffect(const Vector2Int& _index);
	void DeleteGhostEffectByPlayer(const Vector2Int& _index);
	void DeleteGhostEffectByBlock(const Vector2Int& _index);
private:
	GhostEffectController();
	~GhostEffectController();
	void operator=(const GhostEffectController&) = delete;
	void operator&(const GhostEffectController&) = delete;
private:
	StageRegistry* pStageRegistry_ = nullptr;
	std::unordered_map<Vector2Int,std::unique_ptr<GhostEffect>,Vector2IntHash> ghostEffects_;
};