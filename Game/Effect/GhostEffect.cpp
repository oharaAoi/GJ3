#include "GhostEffect.h"

#include "Engine/System/Manager/ParticleManager.h"

GhostEffect::GhostEffect(const Vector2Int& _index):index_(_index){}

GhostEffect::~GhostEffect(){}

void GhostEffect::Start(){
	if(state_ == State::NONE){
		state_ = State::APPEAR;
		behavior_ = std::make_unique<AppearedGhostEffectBehavior>(this);
		behavior_->Init();
	}
}

void GhostEffect::Update(){
	// stateがNONEならUpdateしない
	if(state_ == State::NONE){
		behavior_ = nullptr;
		return;
	}
	if(behavior_){
		behavior_->Update();
	}
}

void GhostEffect::DeleteByPlayer(){
	if(state_ != State::DISAPPEAR_BY_PLAYER){
		state_ = State::DISAPPEAR_BY_PLAYER;
		if(behavior_) behavior_->Finalize();
		behavior_ = std::make_unique<DisappearByPlayerGhostEffectBehavior>(this);
		behavior_->Init();
	}
}

void GhostEffect::DeleteByBlock(){
	if(state_ != State::DISAPPEAR_BY_BLOCK && state_ != State::DISAPPEAR_BY_PLAYER){
		state_ = State::DISAPPEAR_BY_BLOCK;
		if(behavior_) behavior_->Finalize();
		behavior_ = std::make_unique<DisappearByBlockGhostEffectBehavior>(this);
		behavior_->Init();
	}
}

void GhostEffect::TransitionBehavior(std::unique_ptr<IGhostEffectBehavior>&& newBehavior){
	if(behavior_){
		behavior_->Finalize();
	}
	behavior_ = std::move(newBehavior);
	if(behavior_){
		behavior_->Init();
	}
}

void GhostEffect::Debug_Gui(){}

#pragma region "GhostEffect Behavior"
IGhostEffectBehavior::IGhostEffectBehavior(GhostEffect* _host):host_(_host){}
IGhostEffectBehavior::~IGhostEffectBehavior(){}

/// =================================================================
// AppearedGhostEffectBehavior
/// =================================================================
AppearedGhostEffectBehavior::AppearedGhostEffectBehavior(GhostEffect* _host):IGhostEffectBehavior(_host){}
AppearedGhostEffectBehavior::~AppearedGhostEffectBehavior(){}

void AppearedGhostEffectBehavior::Init(){
	// 仮でdustにしておく
	particles_ = ParticleManager::GetInstance()->CrateParticle("dust");
}

void AppearedGhostEffectBehavior::Update(){
	if(!host_){
		return;
	}
	timer_ += GameTimer::DeltaTime();

	// 出現エフェクトが終わったらIdleに移行
	if(timer_ >= host_->appearedEffectTime_){
		host_->state_ = GhostEffect::State::IDLE;
		host_->TransitionBehavior(std::make_unique<IdleGhostEffectBehavior>(host_));
	}
}

void AppearedGhostEffectBehavior::Finalize(){}

/// =================================================================
// IdleGhostEffectBehavior
/// =================================================================
IdleGhostEffectBehavior::IdleGhostEffectBehavior(GhostEffect* _host):IGhostEffectBehavior(_host){}
IdleGhostEffectBehavior::~IdleGhostEffectBehavior(){}

void IdleGhostEffectBehavior::Init(){}

void IdleGhostEffectBehavior::Update(){
	timer_ += GameTimer::DeltaTime();

	timer_ = std::fmod(timer_,host_->idleEffectTime_);
}

void IdleGhostEffectBehavior::Finalize(){}

/// =================================================================
// DisappearByPlayerGhostEffectBehavior
/// =================================================================
DisappearByPlayerGhostEffectBehavior::DisappearByPlayerGhostEffectBehavior(GhostEffect* _host):IGhostEffectBehavior(_host){}
DisappearByPlayerGhostEffectBehavior::~DisappearByPlayerGhostEffectBehavior(){}

void DisappearByPlayerGhostEffectBehavior::Init(){}

void DisappearByPlayerGhostEffectBehavior::Update(){
	timer_ += GameTimer::DeltaTime();

	// 消滅エフェクトが終わったらNONEに移行
	if(timer_ >= host_->disappearByPlayerEffectTime_){
		host_->state_ = GhostEffect::State::NONE;
		Finalize();
	}
}

void DisappearByPlayerGhostEffectBehavior::Finalize(){}

/// =================================================================
// DisappearByBlockGhostEffectBehavior
/// =================================================================
DisappearByBlockGhostEffectBehavior::DisappearByBlockGhostEffectBehavior(GhostEffect* _host):IGhostEffectBehavior(_host){}
DisappearByBlockGhostEffectBehavior::~DisappearByBlockGhostEffectBehavior(){}

void DisappearByBlockGhostEffectBehavior::Init(){}

void DisappearByBlockGhostEffectBehavior::Update(){
	timer_ += GameTimer::DeltaTime();

	// 消滅エフェクトが終わったらNONEに移行
	if(timer_ >= host_->disappearByPlayerEffectTime_){
		host_->state_ = GhostEffect::State::NONE;
		Finalize();
	}
}

void DisappearByBlockGhostEffectBehavior::Finalize(){}

#pragma endregion