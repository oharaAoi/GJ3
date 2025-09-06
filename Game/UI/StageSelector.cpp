#include "StageSelector.h"

/// engine
#include "Engine/System/Input/Input.h"
#include "Engine/System/AUdio/AudioPlayer.h"

/// cmath
#include <cmath>

int32_t StageSelector::currentStageIndex_ = 0;

void StageSelector::Init(){
	uint32_t index = 0;
	for(auto& stagePreview : stagePreviews_){
		stagePreview = std::make_unique<BlockWall>();
		stagePreview->Init(Engine::GetCanvas2d());
		stagePreview->GetSprite()->SetTextureResource(pStageRenderTarget_->GetResource(index));
		stagePreview->GetSprite()->ReSetTextureSize({kWindowWidth_ * 0.3f, kWindowHeight_ * 0.3f});
		index++;
	}

	decidedStage_ = false;

	targetStageIndex_ = StageSelector::currentStageIndex_;
	scrollStartIndex_ = StageSelector::currentStageIndex_;
}
void StageSelector::Debug_Gui(){}

void StageSelector::Update(){
	InputHandle();
	Scroll();
	ConvertIndexToScreen();
}

void StageSelector::InputHandle(){
	Input* input = Input::GetInstance();

	if(decidedStage_){
		return;
	}

	bool isPressing = false;

	if(input->IsControllerConnected()){
		Vector2 leftStick = input->GetLeftJoyStick(0.5f);
		if(leftStick.x < -0.1f){
			isPressing  = true;
			scrollDirection_ = +1;
		} else if(leftStick.x > +0.1f){
			isPressing  = true;
			scrollDirection_ = -1;
		}

		if(input->IsPressButton(kStageIndexSubButtons_)){
			isPressing  = true;
			scrollDirection_ = -1;
		} else if(input->IsPressButton(kStageIndexAddButtons_)){
			isPressing  = true;
			scrollDirection_ = +1;
		}
	}

	for(auto addIndexKey : kStageIndexAddKeys_){
		if(input->IsPressKey(addIndexKey)){
			isPressing  = true;
			scrollDirection_ = +1;
			break;
		}
	}

	if(!isPressing){
		for(auto subIndexKey : kStageIndexSubKeys_){
			if(input->IsPressKey(subIndexKey)){
				isPressing  = true;
				scrollDirection_ = -1;
				break;
			}
		}
	}

	if(isPressing){
		leftPressTime_ -= GameTimer::DeltaTime();
		if(leftPressTime_){
			targetStageIndex_ = (currentStageIndex_ + scrollDirection_ + totalStageNum_) % totalStageNum_;
			if(!isScrolling_){
				AudioPlayer::SinglShotPlay("asobi.mp3", 0.5f);
				scrollTime_ = 0.0f;
				scrollStartIndex_ = currentStageIndex_;

				leftPressTime_ = firstPressInterval_;
			} else{
				leftPressTime_ = pressInterval_;
			}
			isScrolling_ = true;
		}
	} else{
		leftPressTime_ = 0.0f;
	}

	for(auto decideButton : kStageDecideButtons_){
		if(input->IsTriggerButton(decideButton)){
			decidedStage_ = true;
			break;
		}
	}

	for(auto decideKey : kStageDecideKeys_){
		if(input->IsTriggerKey(decideKey)){
			decidedStage_ = true;
			break;
		}
	}
}

void StageSelector::Scroll(){
	if(!isScrolling_){
		currentOffset_ = 0.0f;
		return;
	}

	scrollTime_ += GameTimer::DeltaTime();
	float t = (std::min)(scrollTime_ / scrollDuration_,1.0f);

	// アニメーション進行度から現在インデックスを補間
	float indexF = static_cast<float>(targetStageIndex_) + scrollDirection_ * t;
	if(indexF < 0){
		indexF += totalStageNum_;
	}
	if(indexF >= totalStageNum_){
		indexF -= totalStageNum_;
	}

	currentStageIndex_ = static_cast<int>(std::floor(indexF + 0.5f)); // 四捨五入で更新

	// 描画用オフセット（-1.0 ~ +1.0）
	currentOffset_ = scrollDirection_ * (t * theSpaceBetweenButtons_);

	if(t >= 1.0f){
		currentStageIndex_ = targetStageIndex_;
		currentOffset_ = 0.0f;
		isScrolling_ = false;
	}
}

void StageSelector::ConvertIndexToScreen(){
	/// テクスチャの切り替え用
	//int leftIndex   = (currentStageIndex_ - 1 + totalStageNum_) % totalStageNum_;
	//int centerIndex = currentStageIndex_;
	//int rightIndex  = (currentStageIndex_ + 1) % totalStageNum_;

	// 中央を基準にオフセットでずらす
	float offsetX = currentOffset_;

	stagePreviews_[0]->SetPosition(centerPos_ + Vector2(-theSpaceBetweenButtons_ + offsetX,0.0f)); // 左
	stagePreviews_[1]->SetPosition(centerPos_ + Vector2(offsetX,0.0f)); // 中央
	stagePreviews_[2]->SetPosition(centerPos_ + Vector2(+theSpaceBetweenButtons_ + offsetX,0.0f)); // 右
}

StageSelector::RotateAnimationParam::RotateAnimationParam(){}

StageSelector::RotateAnimationParam::~RotateAnimationParam(){}

json StageSelector::RotateAnimationParam::ToJson(const std::string& id) const{
	JsonBuilder j(id);
	j.Add("duration",duration);
	j.Add("curveSize",static_cast<int>(rotationCurve_.keyframes.size()));
	for(size_t i = 0; i < rotationCurve_.keyframes.size(); ++i){
		j.Add("time" + std::to_string(i),rotationCurve_.keyframes[i].time);
		j.Add("value" + std::to_string(i),rotationCurve_.keyframes[i].value);
	}
	return j.Build();
}
void StageSelector::RotateAnimationParam::FromJson(const json& jsonData){
	fromJson(jsonData,"flashDuration",duration);
	int curveSize = 0;
	fromJson(jsonData,"curveSize",curveSize);
	rotationCurve_.keyframes.clear();
	for(int i = 0; i < curveSize; ++i){
		float time = 0.f;
		float value = 0.f;
		fromJson(jsonData,"time" + std::to_string(i),time);
		fromJson(jsonData,"value" + std::to_string(i),value);
		rotationCurve_.keyframes.push_back({time,value});
	}
}