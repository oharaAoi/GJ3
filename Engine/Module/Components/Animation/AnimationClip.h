#pragma once
#include <vector>
#include <map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Lib/Math/Vector3.h"
#include "Engine/Lib/Math/Quaternion.h"
#include "Engine/Module/Components/Rigging/Skeleton.h"
#include "Engine/Module/Components/Animation/AnimationStructures.h"

class AnimationManager;

/// <summary>
/// Animationに関するクラス
/// </summary>
class AnimationClip {
public:

	AnimationClip();
	~AnimationClip();

	void Init(const std::string& rootName, bool isSkinning, bool isLoop);
	void Update();

	/// <summary>
	/// Animationの読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="animationFile"></param>
	void LoadAnimation(const std::string directoryPath, const std::string& animationFile);
	
	/// <summary>
	/// Animationの適応
	/// </summary>
	/// <param name="skelton"></param>
	void ApplyAnimation(Skeleton* skelton);

	/// <summary>
	/// animetionを遷移させる
	/// </summary>
	/// <param name="skelton">: skelton</param>
	void LerpApplyAnimation(Skeleton* skelton);

	/// <summary>
	/// animationを遷移させる設定をする
	/// </summary>
	/// <param name="preAnimation">: 遷移前アニメーション</param>
	/// <param name="lerpAnimation">: 遷移後アニメーション</param>
	/// <param name="blendSpeed">: 遷移する速さ</param>
	void LerpAnimation(const std::string& preAnimation, const std::string& lerpAnimation, float blendSpeed);

	/// <summary>
	/// Pose状態からのアニメーションを再生する
	/// </summary>
	/// <param name="afterAnimationName"></param>
	void PoseToAnimation(const std::string& afterAnimationName, float lerpTime);

	/// <summary>
	/// animationを遷移させる設定をする
	/// </summary>
	/// <param name="lerpAnimation">: 遷移後アニメーション</param>
	void LerpAnimation(const std::string& lerpAnimation, float blendSpeed);

	/// <summary>
	/// Animationをそのまま切り替える
	/// </summary>
	/// <param name="animationName"></param>
	void ResetAnimation(const std::string& animationName);

	/// <summary>
	/// Animationの予約
	/// </summary>
	/// <param name="preAnimation">: 遷移前アニメーション</param>
	/// <param name="lerpAnimation">: 遷移後アニメーション</param>
	/// <param name="startTransitionRaito">: 前のAnimationがどのくらいの割合の時に開始するか</param>
	void ReservationAnimation(const std::string& preAnimation, const std::string& lerpAnimation, float blendSpeed, float startTransitionRaito);

	/// <summary>
	/// Animationのデバック表示
	/// </summary>
	void Debug_Gui();

public:

	std::string SelectAnimationName();

	// animationの時間を取得・設定
	float GetAnimationTime() const { return animationTime_; }		// 取得
	void SetAnimationTime(const float& time) { animationTime_ = time; }	// 設定

	// animationの最大フレーム数を取得
	const float GetAnimationDuration() const { return animation_.duration; }

	// 今のAnimationの名前を取得する
	const std::string& GetAnimationName() const { return animation_.animationName; }

	// animationの行列を取得
	const Matrix4x4 GetMatrix() const { return animationMat_; }

	// animationが終了しているかを取得
	const bool GetIsAnimationFinish() const { return isAnimationFinish_; }

	// animatinoの切り替えがあるかを取得
	const bool GetIsChange() const { return isAnimationChange_; }

	// animationをloopさせるかを取得・設定
	const bool GetIsLoop() const { return isLoop_; }	// 取得
	void SetIsLoop(bool isLoop) { isLoop_ = isLoop; }	// 設定

	const bool GetIsStop() const { return isStop_; }

	// 予約があるかの取得
	const bool GetIsReservation() const { return isReservation_; }

	// モデルに入っているAnimationの名前を取得
	const std::vector<std::string>& GetAnimationNames() { return animationNames_; }

	// Animationの速度
	void SetAnimationSpeed(float speed) { animationSpeed_ = speed; }

private:

	AnimationManager* manager_;

	// 実際の動き
	Animation animation_;
	std::string nowAnimationName_;
	// アニメーションの時間
	float animationTime_ = 0.0f;
	// アニメーションの速度
	float animationSpeed_ = 1.0f;

	Matrix4x4 animationMat_ = Matrix4x4::MakeUnit();

	std::string rootName_;
	// アニメーションの入っているファイルの名前
	std::string animationFileName_;
	// アニメーションの名前の配列
	std::vector<std::string> animationNames_;

	// skinningを行うか
	bool isSkinnig_ = true;
	// Animation終了フラグ
	bool isAnimationFinish_ = false;
	// アニメーションをループさせるか
	bool isLoop_ = false;
	// アニメーションを止めるかどうか
	bool isStop_ = false;

	int selectedAnimationIndex = 0;

	bool poseToAnimation_ = false;

	// -------------------------------------------------
	// ↓ アニメーションの遷移に関する変数
	// -------------------------------------------------
	bool isAnimationChange_;			// アニメーションの遷移を行うか
	float blendFactor_;					// 補完の線形代数
	float blendSpeed_;
	Animation lerpAnimetion_[2];		// 補完させるアニメーション
	float lerpAnimationTime_[2];		// アニメーションさせるkeyTime
	int lerpAnimationNamesIndex_[2];	// 

	bool isReservation_;				// Animationの予約をする
	float startTransitionRaito_;		// 遷移を開始する割合
};


