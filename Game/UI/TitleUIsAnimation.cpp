#include "TitleUIsAnimation.h"

#include <random> 

#include "Game/UI/TitleUIs.h"

void TitleUIsAnimation::Init(TitleUIs* titleUIs)
{
	titleUIs_ = titleUIs;


    animaParam_[0].finishAngle = 40.0f;
    animaParam_[2].finishAngle = -40.0f;
    animaParam_[3].finishAngle = 40.0f;
}

void TitleUIsAnimation::Update()
{

    randomTimer_ += GameTimer::DeltaTime();

    if (randomTimer_ >= randomInterval_) {
        // 乱数器（スレッドローカル）
        static thread_local std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<int> pickIdx(0, static_cast<int>(animaParam_.size()) - 1);
        std::uniform_real_distribution<float> nextSec(1.5f, 3.5f);

        // すでに再生中でない index を最大 N 回トライして選ぶ
        int index = pickIdx(rng);
        for (int tryCount = 0; tryCount < 8; ++tryCount) {
            if (!animaParam_[index].isAnima) break;
            index = pickIdx(rng);
        }

        AnimationUI(index);                  // ← ランダムに起動
        randomTimer_ = 0.0f;
        randomInterval_ = nextSec(rng);      // ← 次回の起動までの秒数をランダム再設定
    }

    // TitleAnimationの処理
    for (size_t i = 0; i < animaParam_.size(); ++i) {
        if (animaParam_[i].isAnima) {
            if (!animaParam_[i].reMove) {
                animaParam_[i].animaTimer += GameTimer::DeltaTime();
                if (animaParam_[i].animaTimer > 1.0f) {
                    animaParam_[i].reMove = true;
                    const float dir = (animaParam_[i].finishAngle >= 0.0f) ? 1.0f : -1.0f;
                    const float rotFall = FrameFallAngle(1.0f, std::fabs(animaParam_[i].finishAngle)) * dir;
                    animaParam_[i].returnStartAngle = rotFall;
                }
            } else {
                animaParam_[i].animaTimer -= GameTimer::DeltaTime();
            }
            animaParam_[i].animaTimer =
                std::clamp(animaParam_[i].animaTimer, 0.0f, 1.0f);
            // 最初の揺れる処理
            if (!animaParam_[i].reMove) {
                if (i == 1 || i == 4) {
                    float rotate = DampedPendulumAngle01(animaParam_[i].animaTimer);
                    titleUIs_->GetTitleWordsRef()[i]->SetRotate(rotate);
                } else {
                    float rotate = FrameFallAngle(animaParam_[i].animaTimer, animaParam_[i].finishAngle);
                    titleUIs_->GetTitleWordsRef()[i]->SetRotate(rotate);
                }

            // 揺れが終わった後の戻る処理
            } else {
                if (i == 1 || i == 4) {
                    animaParam_[i].animaTimer = 0.0f;
                } else {
                    float rotate = FrameFallAngleReturn(animaParam_[i].animaTimer, animaParam_[i].returnStartAngle,0.0f);
                    titleUIs_->GetTitleWordsRef()[i]->SetRotate(rotate);
                }
            }

            if (animaParam_[i].reMove && animaParam_[i].animaTimer == 0.0f) {
                animaParam_[i].isAnima = false;
            }
        }
    }
}

void TitleUIsAnimation::Debug_Gui()
{
}

void TitleUIsAnimation::AnimationUI(int index)
{
    if (!animaParam_[index].isAnima) {
        animaParam_[index].animaTimer = 0.0f;
        animaParam_[index].isAnima = true;
        animaParam_[index].reMove = false;
    }
}

float TitleUIsAnimation::FrameFallAngle(float time, float finalDeg, float k, float w)
{
    time = std::clamp(time, 0.0f, 1.0f);
    const float finalRad = Radians(finalDeg);

    // 減衰付きで最終角へ収束（軽くビヨン）
    // 左上ヒンジで下へ倒れるので負方向
    float angle = -finalRad * (1.0f - std::exp(-k * time) * std::cos(w * time));
    return angle; // ラジアン
}

float TitleUIsAnimation::FrameFallAngleReturn(float time, float startAngle, float overshootRatio)
{
    // time10 は 1→0 で進行。内部では 0→1 の正方向に変換して扱う
    time = std::clamp(time, 0.0f, 1.0f);
    float u = 1.0f - time;                   // 0..1（戻りの進捗）
    float s = u * u * (3.0f - 2.0f * u);       // smoothstep(0..1)

    // 基本は線形に近い減衰（smoothstepで自然な減速）
    float angle = startAngle * (1.0f - s);

    // オプションのわずかなオーバーシュート（好みで 0.05～0.12 程度）
    if (overshootRatio > 0.0f) {
        float overshoot = startAngle * overshootRatio * std::sin(kPI * u) * (1.0f - s);
        angle += overshoot;
    }
    return angle; // ラジアン
}

float TitleUIsAnimation::DampedPendulumAngle01(float time, float amplitudeDeg, float cycles, float damping)
{
    time = std::clamp(time, 0.0f, 1.0f);

    // 初期角 0, 初期角速度 > 0 を想定：A * e^{-d t} * sin(2π f t)
    const float A = Radians(amplitudeDeg);
    const float angle =
        A * std::exp(-damping * time) *
        std::sin(2.0f * kPI * cycles * time);

    return angle; // ラジアン（AnchorPoint がヒンジ点ならそのまま回せます）
}
