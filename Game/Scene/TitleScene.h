#pragma once
#include "Engine/Render/SceneRenderer.h"
#include "Game/Scene/BaseScene.h"
#include <array>
#include <memory>
// engine
#include "Engine/System/Audio/AudioPlayer.h"
// camera
#include "Game/Camera/DebugCamera.h"
#include "Game/Camera/Camera2d.h"
#include "Game/Camera/Camera3d.h"
#include "Game/WorldObject/Skybox.h"
//ui
#include "Game/UI/TitleUIs.h"
#include "Game/Effect/TitleFlashEffect.h"

class TitleScene :
	public BaseScene{
public:
	static constexpr std::array<uint8_t,2> kTransitionKeys = {DIK_RETURN,DIK_SPACE};
	static constexpr std::array<XInputButtons,2> kTransitionButtons = {XInputButtons::BUTTON_A,XInputButtons::BUTTON_B};
public:
	TitleScene();
	~TitleScene();

	void Init() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;

	void InputHandle();
	void TransitionUpdate();

private:
	// ------------------- camera ------------------- //
	std::unique_ptr<Camera2d> camera2d_ = nullptr;
	std::unique_ptr<Camera3d> camera3d_ = nullptr;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	// ------------------- actor ------------------- //
	Skybox* skybox_;

	// ------------------- ui ------------------- //
	std::unique_ptr<TitleUIs> uis_;
	std::unique_ptr<ThunderFlash> thunderFlash_; // 通常エフェクト

	// ------------------- transition ------------------- //
	bool isTransition_ = false;
	std::unique_ptr<LightFlash> lightFlash_;     // 遷移エフェクト

	// ------------------- audio ------------------- //
	std::unique_ptr<AudioPlayer> bgm_;

	SceneRenderer* sceneRenderer_;
};

