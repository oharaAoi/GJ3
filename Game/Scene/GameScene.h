#pragma once
#include <memory>
#include "Game/Scene/BaseScene.h"
#include "Engine/Render/SceneRenderer.h"
#include "Engine/Module/Components/Effect/BaseParticles.h"
#include "Engine/System/Audio/AudioPlayer.h"
// camera
#include "Game/Camera/Camera3d.h"
#include "Game/Camera/Camera2d.h"
#include "Game/Camera/DebugCamera.h"
// actor
#include "Game/WorldObject/Skybox.h"
#include "Game/Actor/Player/Player.h"
#include "Game/Actor/Effect/GhostSoulManager.h"
#include "Game/Manager/GhostSmokeManager.h"
#include "Game/Manager/StageRegistry.h"
#include "Game/Manager/Collision/Common/MapCollisionSystem.h"
#include "Game/WorldObject/WorldObjects.h"
#include "Game/UI/Menu/MenuSelector.h"
#include "Game/UI/GetGhostCountUI.h"
#include "Game/UI/StageResetUI.h"
#include "Game/UI/Tutorial/TutorialDirector.h"
#include "Game/Effect/SwirlTransition.h"
// ui
#include "Game/UI/GameUIs.h"

// effect
#include "Game/Effect/TitleFlashEffect.h"

class IGameSceneBehavior;

class GameScene
	: public BaseScene{
public:
	// 獲得ゴースト数
	struct Result { int ghostCount = 0; };

	// 読み取り口（ClearScene から使う）
	static const std::optional<Result>& LastResult();
	static void ClearLastResult();

	static constexpr float kResetTime_ = 0.6f; // リセットするまでの時間

public:
	GameScene();
	~GameScene();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:

	void ChengeScene();

	void ChangeBehavior(std::unique_ptr<IGameSceneBehavior> newBehavior);

private:
	// ------------------- camera ------------------- //
	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<Camera3d> camera3d_;
	std::unique_ptr<Camera2d> camera2d_;

	// ------------------- actor ------------------- //

	std::unique_ptr<StageRegistry> stageRegistry_;

	std::unique_ptr<MapCollisionSystem> mapCollision_;

	std::unique_ptr<WorldObjects> worldObjects_;

	std::unique_ptr<Player> player_;

	std::unique_ptr<GhostSoulManager> ghostSoulManager_;

	std::unique_ptr<MenuSelector> menuSelector_;

	std::unique_ptr<GetGhostCountUI> getGhostCountUI_;

	std::unique_ptr<StageResetUI> stageResetUI_;

	std::unique_ptr<TutorialDirector> tutorialDirector_;

	// ------------------- ui ------------------- //
	std::unique_ptr<GameUIs> gameUIs_;

	// ------------------- behavior ------------------- //

	std::unique_ptr<IGameSceneBehavior> behavior_;

	// ------------------- effect ------------------- //
	BaseParticles* orb_;
	BaseParticles* dust_;
	GhostSmokeManager* ghostEffectManager_;

	std::unique_ptr<SwirlTransition> swirlTransition_;

	// ------------------- sound ------------------- //
	std::unique_ptr<AudioPlayer> bgm_;

	float resetTimer_ = 0.0f;			// 現在のリセットまでの時間
	bool isClearConditionMet_ = false;	// クリア条件を満たしたかどうか
	float clearTimer_ = 0.0f;			// クリア時の遷移時間

	SceneRenderer* sceneRenderer_;

	static std::optional<Result> s_lastResult_;
};

class IGameSceneBehavior {
public: 
	IGameSceneBehavior(GameScene* _host) :host_(_host) {}
	virtual ~IGameSceneBehavior() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;

private:
	GameScene* host_ = nullptr;
};

class ChangeSelectSceneBehavior :
	public IGameSceneBehavior {
public:
	ChangeSelectSceneBehavior(GameScene* _host);
	~ChangeSelectSceneBehavior() override;

	void Init() override;
	void Update() override;

private:

	std::unique_ptr<LightFlash> lightFlash_;

};