#pragma once
#include <memory>
#include "Game/Scene/BaseScene.h"
#include "Engine/Render/SceneRenderer.h"
// camera
#include "Game/Camera/Camera3d.h"
#include "Game/Camera/Camera2d.h"
#include "Game/Camera/DebugCamera.h"
// actor
#include "Game/WorldObject/Skybox.h"
#include "Game/Actor/Player/Player.h"
#include "Game/Manager/StageRegistry.h"

class GameScene
	: public BaseScene{
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

	Skybox* skybox_;

	std::unique_ptr<StageRegistry> stageRegistry_;

	std::unique_ptr<Player> player_;

	SceneRenderer* sceneRenderer_;
};
