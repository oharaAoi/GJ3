#pragma once
#include <memory>
#include "Game/Scene/BaseScene.h"
#include "Engine/Render/SceneRenderer.h"
// camera
#include "Game/Camera/Camera3d.h"
#include "Game/Camera/Camera2d.h"
#include "Game/Camera/DebugCamera.h"
// actor
#include "Game/WorldObject/WorldObjects.h"
#include "Game/Actor/Effect/GhostSoulManager.h"
#include "Game/UI/Clear/ClearSelector.h"
#include "Engine/System/Audio/AudioPlayer.h"
#include "Game/Effect/SwirlTransition.h"
// effect
#include "Game/Effect/TitleFlashEffect.h"

class ClearScene : 
	public BaseScene
{
public:

	ClearScene();
	~ClearScene();

	void Init() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;

private:

	void ChengeScene();

private:

	// ------------------- camera ------------------- //
	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<Camera3d> camera3d_;
	std::unique_ptr<Camera2d> camera2d_;

	// ------------------- actor ------------------- //

	int ghostCount_ = 0;

	std::unique_ptr<WorldObjects> worldObjects_;

	std::unique_ptr<GhostSoulManager> ghostSoulManager_;

	std::unique_ptr<ClearSelector> clearSelector_;

	std::unique_ptr<SwirlTransition> swirlTransition_;

	std::unique_ptr<LightFlash> lightFlash_;
	float changeTimer_ = 0.0f;

	std::unique_ptr<AudioPlayer> bgm_;

	SceneRenderer* sceneRenderer_;

};

