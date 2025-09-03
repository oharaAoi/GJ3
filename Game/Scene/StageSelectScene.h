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
#include "Game/WorldObject/WorldObjects.h"
// ui
#include "Game/UI/StageSelector.h"
#include "Game/StageRender/StageContents.h"

class StageSelectScene
	: public BaseScene{
public:
	StageSelectScene();
	~StageSelectScene();

	void Init() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;

private:
	// ------------------- camera ------------------- //
	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<Camera3d> camera3d_;
	std::unique_ptr<Camera2d> camera2d_;

	// ------------------- actor ------------------- //
	Skybox* skybox_;

	std::unique_ptr<WorldObjects> worldObjects_;

	// ------------------- ui ------------------- //
	std::unique_ptr<StageSelector> stageSelector_;

	std::unique_ptr<StageContents> stageContents_;

	SceneRenderer* sceneRenderer_;
};
