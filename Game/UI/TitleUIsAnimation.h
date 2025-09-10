#pragma once
#include <array>

#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Lib/Math/MyMath.h"
#include "Engine/Lib/Math/Vector2.h"

class TitleUIs;

class TitleUIsAnimation :
	public AttributeGui
{
public:

	TitleUIsAnimation() = default;
	~TitleUIsAnimation() = default;

	void Init(TitleUIs* titleUIs);

	void Update();

	void Debug_Gui() override;

	// 0～4を入れる
	void AnimationUI(int index);

private:
	
	/// 0→1 の進行で額縁が片側ヒンジ落ちする回転角(ラジアン)だけ返す
	/// finalDeg: 最終角度（度）。90～120°くらいが自然
	/// k: 減衰の強さ（大きいほど早く収束）
	/// w: ビヨンの周波数（小さいほど揺れ少なめ）
	// 額縁が落ちる処理
	float FrameFallAngle(
		float time,
		float finalDeg = 110.0f,
		float k = 5.0f,
		float w = 9.0f
	);
	// 最終傾き目安
	// 前半の減衰
	// 前半のゆらぎ
	// 前半(倒れる)に使う割合 0.5～0.8 推奨
	float FrameFallAngleReturn(
		float time,
		float startAngle,
		float overshootRatio = 0.0f
	);


	// 0..1 の正規化時間
	// 初回ピークのおおよその振れ幅（度）
	// 0..1 のあいだに何回 揺れるか（周回数）
	// 減衰の強さ（大きいほど早く収束）
	// 振り子のように揺れる処理
	float DampedPendulumAngle01(
		float time,
		float amplitudeDeg = 25.0f,
		float cycles = 2.5f,
		float damping = 4.0f
	);

	float Radians(float deg) { return deg * kPI / 180.0f; }

private:

	// タイトルのUI
	TitleUIs* titleUIs_ = nullptr;

	// 全体の尺
	float animaTimer_;


	struct AnimaParam {
		bool isAnima = false;
		bool reMove = false;
		float finishAngle = 0.0f;
		float animaTimer = 0.0f;
		float returnStartAngle = 0.0f;
	};
	// そのUIはAnimationを行っているか？
	std::array<AnimaParam, 5> animaParam_;

	float randomTimer_ = 0.0f;
	float randomInterval_ = 2.0f;

};

