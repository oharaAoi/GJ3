#pragma once

#include "Engine/Module/Components/Attribute/AttributeGui.h"

#include "Engine/Module/Components/Effect/BaseParticles.h"
#include "Engine/Module/Components/2d/Sprite.h"

#include "Engine/Lib/Math/Vector2Int.h"

class IGhostEffectBehavior;

/// <summary>
/// オバケが出た時のエフェクト
/// </summary>
class GhostEffect
	: public AttributeGui{
	friend class AppearedGhostEffectBehavior;
	friend class IdleGhostEffectBehavior;
	friend class DisappearByPlayerGhostEffectBehavior;
	friend class DisappearByBlockGhostEffectBehavior;
public:
	enum class State{
		NONE,     // 無し
		APPEAR,   // 出現
		IDLE,     // 待機
		DISAPPEAR_BY_PLAYER, // プレイヤーに取得されて消滅
		DISAPPEAR_BY_BLOCK   // ブロックが離れて消滅
	};
public:
	GhostEffect(const Vector2Int& _index);
	~GhostEffect() override;

	void Update();
	
	void Debug_Gui() override;

	void Start();
	void End();

	/// <summary>
	/// プレイヤーに取得されて消滅する
	/// </summary>
	void DeleteByPlayer();
	/// <summary>
	/// ブロックが離れて消滅する
	/// </summary>
	void DeleteByBlock();

private:
	Vector2Int index_ = {-1,-1};
	State state_ = State::NONE;
	// 特殊ブロックに影響を与える範囲のSprite
	std::shared_ptr<Sprite> ghostRangeEffectSprite_ = nullptr;

	std::unique_ptr<IGhostEffectBehavior> behavior_ = nullptr;

	/// <summary>
	/// 各エフェクトの時間
	/// </summary>
	float appearedEffectTime_ = 1.f; // sec
	float idleEffectTime_ = 5.f; // sec
	float disappearByPlayerEffectTime_ = 1.f; // sec
	float disappearByBlockEffectTime_ = 1.f; // sec
public:
	const Vector2Int& GetIndex() const { return index_; }
	State GetState() const{ return state_; }

	void TransitionBehavior(std::unique_ptr<IGhostEffectBehavior>&& newBehavior);
};

class IGhostEffectBehavior{
public:
	IGhostEffectBehavior(GhostEffect* _host);
	virtual ~IGhostEffectBehavior();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Finalize() = 0;

protected:
	GhostEffect* host_ = nullptr;
	float timer_ = 0.f;
};

/// 出現時の挙動
class AppearedGhostEffectBehavior
	: public IGhostEffectBehavior{
public:
	AppearedGhostEffectBehavior(GhostEffect* _host);
	~AppearedGhostEffectBehavior() override;
	void Init() override;
	void Update() override;
	void Finalize() override;
private:
	BaseParticles* particles_ = nullptr;
};

/// 通常時  挙動
class IdleGhostEffectBehavior
	: public IGhostEffectBehavior{
public:
	IdleGhostEffectBehavior(GhostEffect* _host);
	~IdleGhostEffectBehavior() override;
	void Init() override;
	void Update() override;
	void Finalize() override;
private:
	BaseParticles* particles_ = nullptr;
};

/// プレイヤーに取得されて消滅する時の挙動
class DisappearByPlayerGhostEffectBehavior
	: public IGhostEffectBehavior{
public:
	DisappearByPlayerGhostEffectBehavior(GhostEffect* _host);
	~DisappearByPlayerGhostEffectBehavior() override;
	void Init() override;
	void Update() override;
	void Finalize() override;
};

/// ブロックが離れて消滅する時の挙動
class DisappearByBlockGhostEffectBehavior
	: public IGhostEffectBehavior{
public:
	DisappearByBlockGhostEffectBehavior(GhostEffect* _host);
	~DisappearByBlockGhostEffectBehavior() override;
	void Init() override;
	void Update() override;
	void Finalize() override;
};
