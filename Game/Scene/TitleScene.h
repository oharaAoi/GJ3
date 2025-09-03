#pragma once
#include "Engine/Render/SceneRenderer.h"
#include "Game/Scene/BaseScene.h"
#include <array>
#include <memory>
// camera
#include "Game/Camera/DebugCamera.h"
#include "Game/Camera/Camera2d.h"
#include "Game/Camera/Camera3d.h"
#include "Game/WorldObject/Skybox.h"
//ui
#include "Game/UI/TitleUIs.h"

class TitleScene :
	public BaseScene{
public:
	static constexpr std::array<uint8_t,2> kTransitionKeys = {DIK_RETURN,DIK_SPACE};
	static constexpr std::array<XInputButtons,2> kTransitionButtons = {XInputButtons::BUTTON_A,XInputButtons::BUTTON_B};
public:
	TitleScene();
	~TitleScene();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:
	// ------------------- camera ------------------- //
	std::unique_ptr<Camera2d> camera2d_ = nullptr;
	std::unique_ptr<Camera3d> camera3d_ = nullptr;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	// ------------------- actor ------------------- //
	Skybox* skybox_;

	std::unique_ptr<TitleUIs> uis_;

	SceneRenderer* sceneRenderer_;
};

