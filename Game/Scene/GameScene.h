#pragma once
#include <memory>
#include "Game/Scene/BaseScene.h"
#include "Engine/Render/SceneRenderer.h"
#include "Engine/Module/Components/Effect/BaseParticles.h"
// camera
#include "Game/Camera/Camera3d.h"
#include "Game/Camera/Camera2d.h"
#include "Game/Camera/DebugCamera.h"
// actor
#include "Game/WorldObject/Skybox.h"
#include "Game/Actor/Player/Player.h"
#include "Game/Manager/StageRegistry.h"
#include "Game/WorldObject/WorldObjects.h"
#include "Game/Manager/Collision/Common/MapCollisionSystem.h"

class GameScene
	: public BaseScene{
public:
	static constexpr float kResetTime_ = 0.6f; // リセットするまでの時間

public:
	GameScene();
	~GameScene();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

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

	BaseParticles* orb_;
	BaseParticles* dust_;

	float resetTimer_ = 0.0f; // 現在のリセットまでの時間

	SceneRenderer* sceneRenderer_;
};
