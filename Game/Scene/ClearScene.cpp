#include "ClearScene.h"
#include "Engine.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Game/Scene/GameScene.h"
#include "Game/UI/StageSelect/StageSelector.h"
#include "Game/Manager/GhostEffectManager.h"

ClearScene::ClearScene(){}

ClearScene::~ClearScene() { Finalize(); }

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void ClearScene::Init()
{
	JsonItems* adjust = JsonItems::GetInstance();
	adjust->Init("StageSelectScene");

	// -------------------------------------------------
	// ↓ Sceneの初期化
	// -------------------------------------------------

	sceneRenderer_ = SceneRenderer::GetInstance();
	sceneRenderer_->Init();
	EditorWindows::GetInstance()->SetSceneRenderer(sceneRenderer_);

	// -------------------------------------------------
	// ↓ cameraの初期化
	// -------------------------------------------------

	camera3d_ = std::make_unique<Camera3d>();
	debugCamera_ = std::make_unique<DebugCamera>();
	camera2d_ = std::make_unique<Camera2d>();
	camera3d_->Init();
	debugCamera_->Init();
	camera2d_->Init();

	// -------------------------------------------------
	// ↓ actorの初期化
	// -------------------------------------------------

	// GameSceneからの値を取得
	if (const auto& opt = GameScene::LastResult()) {
		ghostCount_ = opt->ghostCount;
	}

	ghostSoulManager_ = std::make_unique<GhostSoulManager>();
	ghostSoulManager_->InitClearScene(Engine::GetCanvas2d());
	for (int i = 0; i < ghostCount_; ++i) {
		ghostSoulManager_->CreateSoul(Vector2{ 64.0f,64.0f }, true);
	}
	ghostSoulManager_->SetPosition(Vector2{ 640.0f,360.0f });

	worldObjects_ = std::make_unique<WorldObjects>();
	worldObjects_->Init();
	//worldObjects_->SetTexture("clear.png");

	clearSelector_ = std::make_unique<ClearSelector>();
	clearSelector_->Init();

	bgm_ = std::make_unique<AudioPlayer>();
	bgm_->Init("Clear.mp3");
	bgm_->Play(true, 0.5f);

}

void ClearScene::Finalize()
{
	sceneRenderer_->Finalize();
	GhostEffectManager::GetInstance()->Finalize();
	ParticleManager::GetInstance()->Finalize();
	GpuParticleManager::GetInstance()->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void ClearScene::Update()
{
	// -------------------------------------------------
	// ↓ actorの更新
	// -------------------------------------------------

	worldObjects_->Update();

	ghostSoulManager_->UpdateClearScene();

	clearSelector_->Update();
	ChengeScene();

	// -------------------------------------------------
	// ↓ cameraの更新
	// -------------------------------------------------
	if (debugCamera_->GetIsActive()) {
		debugCamera_->Update();
	} else {
		camera3d_->Update();
	}
	camera2d_->Update();

	// -------------------------------------------------
	// ↓ sceneの更新
	// -------------------------------------------------
	sceneRenderer_->Update();

	// -------------------------------------------------
	// ↓ あたり判定
	// -------------------------------------------------

	// -------------------------------------------------
	// ↓ 最後に行いたい更新
	// -------------------------------------------------

	sceneRenderer_->PostUpdate();
}

void ClearScene::Draw() const
{
	// Sceneの描画
	sceneRenderer_->Draw();
}

void ClearScene::ChengeScene()
{
	// ステージ選択されているか
	if (clearSelector_->GetChangeScene() || clearSelector_->GetChangeStage()) {
		// 遷移の実行が可能か
		if (clearSelector_->GetIsExecute()) {
			// セレクトシーンに遷移
			if (clearSelector_->GetChangeScene()) {
				nextSceneType_ = SceneType::STAGE_SELECT;
			}
			// 次のステージに遷移
			if (clearSelector_->GetChangeStage()) {
				nextSceneType_ = SceneType::GAME;
				int32_t index = StageSelector::GetCurrentStageIndex() + 1;
				StageSelector::SetCurrentStageIndex(index);
			}
		}
	}
}