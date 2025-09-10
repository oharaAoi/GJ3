#include "StageSelector.h"

/// engine
#include "Engine/System/Input/Input.h"
#include "Engine/System/AUdio/AudioPlayer.h"

#include "Engine/Utilities/Timeline.h"

/// math
#include <cmath>
#include "Engine/Lib/Math/MyMath.h"

int32_t StageSelector::currentStageIndex_ = 0;

void StageSelector::Init(){
	SetName("StageSelector");
	/// ==============================
	// arrows
	/// ==============================
	arrows_[0] = Engine::GetCanvas2d()->AddSprite("Select_arrow_left.png","Arrow_Left","Sprite_Normal.json",5,true);
	arrows_[1] = Engine::GetCanvas2d()->AddSprite("Select_arrow_right.png","Arrow_Right","Sprite_Normal.json",5,true);
	arrows_[0]->SetTranslate({84.f,268.7f});
	arrows_[1]->SetTranslate({1196.f,268.7f});

	leftArrowRotateParam_ = std::make_unique<RotateAnimationParam>();
	rightArrowRotateParam_ = std::make_unique<RotateAnimationParam>();
	leftArrowRotateParam_->Init();
	rightArrowRotateParam_->Init();

	leftArrowRotateParam_->FromJson(JsonItems::GetData("RotateAnimation","Left_Arrow"));
	rightArrowRotateParam_->FromJson(JsonItems::GetData("RotateAnimation","Right_Arrow"));

	AddChild(arrows_[0]);
	AddChild(arrows_[1]);
	arrows_[0]->ApplySaveData();
	arrows_[1]->ApplySaveData();

	/// ==============================
	// others
	/// ==============================
	decidedStage_ = false;

	currentIndexF_ = static_cast<float>(StageSelector::currentStageIndex_);
	scrollT_ = currentIndexF_;

	currentStageIndex_ = 0;

	EditorWindows::AddObjectWindow(this,GetName());
}
void StageSelector::Debug_Gui(){
	ImGui::Text("currentIndex : %d",currentStageIndex_);
	ImGui::DragFloat("firstPressInterval",&firstPressInterval_,0.01f,0.0f,1.0f);
	ImGui::DragFloat("pressInterval",&pressInterval_,0.01f,0.0f,1.0f);

	ImGui::Spacing();

	ImGui::DragFloat("ScrollSpeed",&scrollSpeed_,0.01f,0.0f);

	ImGui::Spacing();

	ImGui::DragFloat("theSpaceBetweenButtons",&theSpaceBetweenButtons_,1.0f,0.0f,2000.0f);
	ImGui::DragFloat2("centerPos",&centerPos_.x,1.0f,0.0f,2000.0f);
	ImGui::DragFloat("currentOffsetX",&currentOffsetX_,1.0f,-2000.0f,2000.0f);
	ImGui::DragFloat("offsetY",&offsetY_,0.01f);

	ImGui::Spacing();

	if(ImGui::TreeNode("LeftArrow_AngleEvent")){
		ImGui::DragFloat("Duration##LeftArrow_AngleEvent",&leftArrowRotateParam_->duration,0.01f);
		ImGui::SliderFloat("ElapsedTime##LeftArrow_AngleEvent",&leftArrowRotateParam_->elapsedTime,0.0f,leftArrowRotateParam_->duration);
	#ifdef _DEBUG
		ImGui::EditKeyFrame("LeftArrow_AngleEvent",leftArrowRotateParam_->angleEvent_,leftArrowRotateParam_->duration,0.0f);
	#endif // _DEBUG

		ImGui::Spacing();

		ImGui::DragFloat2("Anchor##LeftArrow_AngleEvent",&leftArrowRotateParam_->anchor.x,0.1f);
		ImGui::DragFloat("Gravity##LeftArrow_AngleEvent",&leftArrowRotateParam_->gravity,0.1f);
		ImGui::DragFloat("Length##LeftArrow_AngleEvent",&leftArrowRotateParam_->length,0.1f);

		ImGui::TreePop();
	}
	if(ImGui::TreeNode("RightArrow_AngleEvent")){
		ImGui::DragFloat("Duration##RightArrow_AngleEvent",&rightArrowRotateParam_->duration,0.01f);
		ImGui::SliderFloat("ElapsedTime##RightArrow_AngleEvent",&rightArrowRotateParam_->elapsedTime,0.0f,rightArrowRotateParam_->duration);
	#ifdef _DEBUG
		ImGui::EditKeyFrame("RightArrow_AngleEvent",rightArrowRotateParam_->angleEvent_,rightArrowRotateParam_->duration,0.0f);
	#endif // _DEBUG
		ImGui::Spacing();

		ImGui::DragFloat2("Anchor##RightArrow_AngleEvent",&rightArrowRotateParam_->anchor.x,0.1f);
		ImGui::DragFloat("Gravity##RightArrow_AngleEvent",&rightArrowRotateParam_->gravity,0.1f);
		ImGui::DragFloat("Length##RightArrow_AngleEvent",&rightArrowRotateParam_->length,0.1f);

		ImGui::TreePop();
	}
}

void StageSelector::Update(){
	InputHandle();
	Scroll();

	leftArrowRotateParam_->Update(arrows_[0]);
	rightArrowRotateParam_->Update(arrows_[1]);

	for (size_t i = 0; i < param_.size(); ++i) {
		if (param_[i].isAnima) {
			param_[i].animaTimer += GameTimer::DeltaTime() * 2.5f;
			param_[i].animaTimer = std::clamp(param_[i].animaTimer, 0.0f, 1.0f);
			float rotate = DampedPendulumAngle(param_[i].animaTimer);
			arrows_[i]->SetRotate(rotate);
			if (param_[i].animaTimer == 1.0f) {
				param_[i].isAnima = false;
				param_[i].animaTimer = 0.0f;
			}
		}
	}
}

// StageSelector.cpp
void StageSelector::InputHandle(){
	Input* input = Input::GetInstance();


	if(decidedStage_){
		return;
	}

	for(auto decideButton : kStageDecideButtons_){
		if(input->IsTriggerButton(decideButton)){
			decidedStage_ = true;
			AudioPlayer::SinglShotPlay("button.mp3",0.5f);
			return;
		}
	}

	for(auto decideKey : kStageDecideKeys_){
		if(input->IsTriggerKey(decideKey)){
			decidedStage_ = true;
			AudioPlayer::SinglShotPlay("button.mp3",0.5f);
			return;
		}
	}

	bool isPressing = false;

	inputScrollDirection_ = 0;
	if(input->IsControllerConnected()){
		Vector2 leftStick = input->GetLeftJoyStick(0.5f);
		if(leftStick.x < -0.1f){
			isPressing  = true;
			inputScrollDirection_ = -1;
			param_[0].isAnima = true;
		} else if(leftStick.x > +0.1f){
			isPressing  = true;
			inputScrollDirection_ = +1;
			param_[1].isAnima = true;
		}

		if(input->IsPressButton(kStageIndexSubButtons_)){
			isPressing  = true;
			inputScrollDirection_ = -1;
			param_[0].isAnima = true;
		} else if(input->IsPressButton(kStageIndexAddButtons_)){
			isPressing  = true;
			inputScrollDirection_ = +1;
			param_[1].isAnima = true;
		}
	}

	for(auto addIndexKey : kStageIndexAddKeys_){
		if(input->IsPressKey(addIndexKey)){
			isPressing  = true;
			inputScrollDirection_ = +1;
			param_[1].isAnima = true;
			break;
		}
	}

	if(!isPressing){
		for(auto subIndexKey : kStageIndexSubKeys_){
			if(input->IsPressKey(subIndexKey)){
				isPressing  = true;
				inputScrollDirection_ = -1;
				param_[0].isAnima = true;
				break;
			}
		}
	}

	leftPressTime_ -= GameTimer::DeltaTime();
	leftPressTime_ = (std::max)(0.f,leftPressTime_);

	if(!isPressing){
		return;
	}

	if(leftPressTime_ > 0.f){
		return;
	}

	AudioPlayer::SinglShotPlay("button.mp3",0.5f);

	// インデックスを更新
	currentIndexF_ += static_cast<float>(inputScrollDirection_);

	leftPressTime_ = isScrolling_ ? pressInterval_ : firstPressInterval_;

	// ループさせる（floatなのでfmodを使う）
	if(currentIndexF_ < 0){
		currentIndexF_ += totalStageNum_;
	}
	if(currentIndexF_ >= totalStageNum_){
		currentIndexF_ -= totalStageNum_;
	}
}

void StageSelector::Scroll(){
	float dt = GameTimer::DeltaTime();
	float totalStageF = static_cast<float>(totalStageNum_);
	float halfStageF  = totalStageF * 0.5f;

	// currentIndexF_ は「目標のfloatインデックス」
	// scrollT_ は「現在のfloatインデックス（現在位置）」
	float delta = currentIndexF_ - scrollT_;

	// 最短経路に補正（ラップを考慮）
	while(delta > halfStageF) delta -= totalStageF;
	while(delta <= -halfStageF) delta += totalStageF;

	// 定速移動（オーバーシュートしない）
	float maxStep = scrollSpeed_ * dt; // scrollSpeed_ : index / 秒
	float step = 0.0f;
	if(std::fabs(delta) <= maxStep){
		step = delta; // 到達可能ならその分だけ動かす
	} else{
		step = (delta > 0.0f) ? maxStep : -maxStep;
	}

	// 方向
	scrollDirection_ = (step > 0.0f) ? 1 : ((step < 0.0f) ? -1 : 0);

	// 実際に進める
	scrollT_ += step;

	// ラップ処理（0..totalStageF)
	if(scrollT_ < 0.0f) scrollT_ += totalStageF;
	if(scrollT_ >= totalStageF) scrollT_ -= totalStageF;

	// 表示上の「中央に一番近いインデックス」を更新
	int centerIdx = static_cast<int>(std::lround(scrollT_)) % totalStageNum_;
	if(centerIdx < 0) centerIdx += totalStageNum_;
	currentStageIndex_ = centerIdx;

	if(pStageContents_){
		pStageContents_->ResetCurrentDrawIndex(currentStageIndex_);
	}

	// currentOffsetX_ は "centerIdx - scrollT_" の符号付き差分に space を掛けたものにする
	// （従来の floor ベースの frac ではなく、round を基準にする）
	float signedDiff = static_cast<float>(centerIdx) - scrollT_;
	// signedDiff も最短経路に補正しておく
	if(signedDiff > halfStageF)  signedDiff -= totalStageF;
	if(signedDiff < -halfStageF) signedDiff += totalStageF;

	currentOffsetX_ = signedDiff * theSpaceBetweenButtons_;
}

float StageSelector::DampedPendulumAngle(float time, float amplitudeDeg, float cycles, float damping)
{
	time = std::clamp(time, 0.0f, 1.0f);

	// 初期角 0, 初期角速度 > 0 を想定：A * e^{-d t} * sin(2π f t)
	const float A = Radians(amplitudeDeg);
	const float angle =
		A * std::exp(-damping * time) *
		std::sin(2.0f * kPI * cycles * time);

	return angle; // ラジアン（AnchorPoint がヒンジ点ならそのまま回せます）
}

#pragma region Pendulum

StageSelector::RotateAnimationParam::RotateAnimationParam(){}

StageSelector::RotateAnimationParam::~RotateAnimationParam(){}

void StageSelector::RotateAnimationParam::Init(){
	elapsedTime = 0.f;
}
void StageSelector::RotateAnimationParam::Update(Sprite* _sprite){
	elapsedTime += GameTimer::DeltaTime();

	preAngleEventIndex_ = currentAngleEventIndex_;

	// ループする
	if(elapsedTime >= duration){
		elapsedTime = 0.f;
		preAngleEventIndex_ = -1;
	}

	currentAngleEventIndex_ = CalculateEventIndex();

	if(currentAngleEventIndex_ == -1){
		return;
	}

	if(currentAngleEventIndex_ != preAngleEventIndex_){
		// 変化したとき
		angle_ = angleEvent_.keyframes[currentAngleEventIndex_].value;
	}

	Vector3 spritePos = PendulumUpdate(anchor,length,angle_,angleVelo_,gravity);
	_sprite->GetTransform()->SetTranslate({spritePos.x,spritePos.y});

	if(std::isnan(angle_)){
		angle_ = 0.f;
	}

	_sprite->GetTransform()->SetRotateZ(angle_);

	_sprite->GetTransform()->GetTransform();
}

int32_t StageSelector::RotateAnimationParam::CalculateEventIndex()const{
	///===========================================
	/// 例外処理
	///===========================================
	if(angleEvent_.keyframes.empty()){
		return -1;
	}
	if(angleEvent_.keyframes.size() == 1){
		if(elapsedTime <= angleEvent_.keyframes[0].time){
			return -1;
		}
		return 0;
	}

	for(int32_t index = 0; index < angleEvent_.keyframes.size() - 1; ++index){
		int32_t nextIndex = index + 1;
		// index と nextIndex の 2つを
		// 取得して 現時刻が 範囲内か
		if(angleEvent_.keyframes[index].time <= elapsedTime && elapsedTime <= angleEvent_.keyframes[nextIndex].time){
			return index;
		}
	}
	// 登録されている時間より 後ろ
	// 最後の 値を返す
	return static_cast<int32_t>(angleEvent_.keyframes.size() - 1);
}

json StageSelector::RotateAnimationParam::ToJson(const std::string& id) const{
	JsonBuilder j(id);
	j.Add("duration",duration);
	j.Add("curveSize",static_cast<int>(this->angleEvent_.keyframes.size()));
	for(size_t i = 0; i < angleEvent_.keyframes.size(); ++i){
		j.Add("time" + std::to_string(i),angleEvent_.keyframes[i].time);
		j.Add("value" + std::to_string(i),angleEvent_.keyframes[i].value);
	}
	j.Add("anchor",anchor);
	j.Add("gravity",gravity);
	j.Add("length",length);

	return j.Build();
}
void StageSelector::RotateAnimationParam::FromJson(const json& jsonData){
	fromJson(jsonData,"flashDuration",duration);
	int curveSize = 0;
	fromJson(jsonData,"curveSize",curveSize);
	angleEvent_.keyframes.clear();

	for(int i = 0; i < curveSize; ++i){
		float time = 0.f;
		float value = 0.f;
		fromJson(jsonData,"time" + std::to_string(i),time);
		fromJson(jsonData,"value" + std::to_string(i),value);
		angleEvent_.keyframes.push_back({time,value});
	}

	fromJson(jsonData,"anchor",anchor);
	fromJson(jsonData,"gravity",gravity);
	fromJson(jsonData,"length",length);

}

#pragma endregion