#include "TitleFlashEffect.h"

/// stl
#include <algorithm>
#include <cassert>

/// engine
#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Engine/Lib/Json/JsonItems.h"

#include "Engine/Utilities/Timeline.h"

#pragma region ThunderFlash

json ThunderFlash::Parameter::ToJson(const std::string& id) const{
	JsonBuilder j(id);
	j.Add("flashDuration",flashDuration_);
	j.Add("curveSize",static_cast<int>(flashCurve_.keyframes.size()));
	for(size_t i = 0; i < flashCurve_.keyframes.size(); ++i){
		j.Add("time" + std::to_string(i),flashCurve_.keyframes[i].time);
		j.Add("value" + std::to_string(i),flashCurve_.keyframes[i].value);
	}
	return j.Build();
}
void ThunderFlash::Parameter::FromJson(const json& jsonData){
	fromJson(jsonData,"flashDuration",flashDuration_);
	int curveSize = 0;
	fromJson(jsonData,"curveSize",curveSize);
	flashCurve_.keyframes.clear();
	for(int i = 0; i < curveSize; ++i){
		float time = 0.f;
		float value = 0.f;
		fromJson(jsonData,"time" + std::to_string(i),time);
		fromJson(jsonData,"value" + std::to_string(i),value);
		flashCurve_.keyframes.push_back({time,value});
	}
}

void ThunderFlash::Init(){
	SetName("ThunderFlash");

	// 読み込み
	parameter_.FromJson(JsonItems::GetData("TitleScene","ThunderFlash"));

	flashOverlaySprite_ = Engine::GetCanvas2d()->AddSprite("white.png", GetName(), "Sprite_Normal.json",3);

	flashOverlaySprite_->SetColor({0.84f,0.83f,89.f,1.f});

	flashOverlaySprite_->SetScale(Vector2(1920.f,1080.f));

	AddChild(flashOverlaySprite_);
	EditorWindows::AddObjectWindow(this,GetName());
}

void ThunderFlash::Finalize(){
	flashOverlaySprite_ = nullptr;
	parameter_.flashCurve_.keyframes.clear();
}

float ThunderFlash::CalculateCurrentFlushAlpha()const{
	///===========================================
	/// 例外処理
	///===========================================
	if(parameter_.flashCurve_.keyframes.empty()){
		return 0.f;
	}
	if(parameter_.flashCurve_.keyframes.size() == 1 || parameter_.currentFlashTime_ <= parameter_.flashCurve_.keyframes[0].time){
		return parameter_.flashCurve_.keyframes[0].value;
	}

	for(size_t index = 0; index < parameter_.flashCurve_.keyframes.size() - 1; ++index){
		size_t nextIndex = index + 1;
		// index と nextIndex の 2つを
		// 取得して 現時刻が 範囲内か
		if(parameter_.flashCurve_.keyframes[index].time <= parameter_.currentFlashTime_ && parameter_.currentFlashTime_ <= parameter_.flashCurve_.keyframes[nextIndex].time){
			// 範囲内 で 保管
			float t = (parameter_.currentFlashTime_ - parameter_.flashCurve_.keyframes[index].time) / (parameter_.flashCurve_.keyframes[nextIndex].time - parameter_.flashCurve_.keyframes[index].time);
			return std::lerp(parameter_.flashCurve_.keyframes[index].value,parameter_.flashCurve_.keyframes[nextIndex].value,t);
		}
	}
	// 登録されている時間より 後ろ
	// 最後の 値を返す
	return (*parameter_.flashCurve_.keyframes.rbegin()).value;
}

void ThunderFlash::Update(){
	parameter_.currentFlashTime_ += GameTimer::DeltaTime();
	// ループする
	if(parameter_.currentFlashTime_ >= parameter_.flashDuration_){
		parameter_.currentFlashTime_ = 0.0f;
	}

	// alpha値を取得
	Vector4 color = flashOverlaySprite_->GetColor();
	alpha_ = CalculateCurrentFlushAlpha();
	/*color.w = (std::min)(alpha_ * 0.4f,1.f);
	flashOverlaySprite_->SetColor(color);

	flashOverlaySprite_->Update();*/
}

void ThunderFlash::Debug_Gui(){
	if(ImGui::Button("Save")){
		JsonItems::Save("TitleScene",parameter_.ToJson("ThunderFlash"));
	}

	float prevDuration = parameter_.flashDuration_;
	if(ImGui::DragFloat("Duration",&parameter_.flashDuration_,0.01f)){
		// 比率を維持する
		for(auto& keyframe : parameter_.flashCurve_.keyframes){
			keyframe.time = (keyframe.time / prevDuration) * parameter_.flashDuration_;
		}
	}
	ImGui::SliderFloat("CurrentTime",&parameter_.currentFlashTime_,0.0f,parameter_.flashDuration_);

	ImGui::Text("FlashCurve");
#ifdef _DEBUG
	ImGui::EditKeyFrame(
		"FlashCurve",
		parameter_.flashCurve_,
		parameter_.flashDuration_,
		0.0f,
		nullptr);
#endif // _DEBUG
}

#pragma endregion

#pragma region LightFlash
json LightFlash::Parameter::ToJson(const std::string& id) const{
	JsonBuilder j(id);
	j.Add("flashDuration",flashDuration_);
	j.Add("curveSize",static_cast<int>(flashCurve_.keyframes.size()));
	for(size_t i = 0; i < flashCurve_.keyframes.size(); ++i){
		j.Add("time" + std::to_string(i),flashCurve_.keyframes[i].time);
		j.Add("value" + std::to_string(i),flashCurve_.keyframes[i].value);
	}
	return j.Build();
}
void LightFlash::Parameter::FromJson(const json& jsonData){
	fromJson(jsonData,"flashDuration",flashDuration_);
	int curveSize = 0;
	fromJson(jsonData,"curveSize",curveSize);
	flashCurve_.keyframes.clear();
	for(int i = 0; i < curveSize; ++i){
		float time = 0.f;
		float value = 0.f;
		fromJson(jsonData,"time" + std::to_string(i),time);
		fromJson(jsonData,"value" + std::to_string(i),value);
		flashCurve_.keyframes.push_back({time,value});
	}
}
void LightFlash::Init(){
	SetName("LightFlash");

	isFinish_ = false;

	// 読み込み
	parameter_.FromJson(JsonItems::GetData("TitleScene","LightFlash"));

	flashOverlaySprite_ = Engine::GetCanvas2d()->AddSprite("white.png", GetName(), "Sprite_Normal.json",6);
	flashOverlaySprite_->SetAnchorPoint({1.f,1.f});
	flashOverlaySprite_->SetScale(Vector2(1920.f,1080.f));
	flashOverlaySprite_->SetColor(Vector4(0.f,0.f,0.f,0.f));

	AddChild(flashOverlaySprite_);
	EditorWindows::AddObjectWindow(this,GetName());
}
void LightFlash::Finalize(){
	flashOverlaySprite_ = nullptr;
	parameter_.flashCurve_.keyframes.clear();
}
float LightFlash::CalculateCurrentFlushAlpha()const{
	///===========================================
	/// 例外処理
	///===========================================
	if(parameter_.flashCurve_.keyframes.empty()){
		return 0.f;
	}
	if(parameter_.flashCurve_.keyframes.size() == 1 || parameter_.currentFlashTime_ <= parameter_.flashCurve_.keyframes[0].time){
		return parameter_.flashCurve_.keyframes[0].value;
	}
	for(size_t index = 0; index < parameter_.flashCurve_.keyframes.size() - 1; ++index){
		size_t nextIndex = index + 1;
		// index と nextIndex の 2つを
		// 取得して 現時刻が 範囲内か
		if(parameter_.flashCurve_.keyframes[index].time <= parameter_.currentFlashTime_ && parameter_.currentFlashTime_ <= parameter_.flashCurve_.keyframes[nextIndex].time){
			// 範囲内 で 保管
			float t = (parameter_.currentFlashTime_ - parameter_.flashCurve_.keyframes[index].time) / (parameter_.flashCurve_.keyframes[nextIndex].time - parameter_.flashCurve_.keyframes[index].time);
			return std::lerp(parameter_.flashCurve_.keyframes[index].value,parameter_.flashCurve_.keyframes[nextIndex].value,t);
		}
	}
	// 登録されている時間より 後ろ
	// 最後の 値を返す
	return (*parameter_.flashCurve_.keyframes.rbegin()).value;
}

void LightFlash::Update(){
	parameter_.currentFlashTime_ += GameTimer::DeltaTime();
	// ループしない
	if(parameter_.currentFlashTime_ >= parameter_.flashDuration_){
		parameter_.currentFlashTime_ = parameter_.flashDuration_;
		isFinish_ = true;
	}
	// alpha値を取得
	float alpha = CalculateCurrentFlushAlpha();
	flashOverlaySprite_->SetColor(Vector4(0.f,0.f,0.f,alpha));
	flashOverlaySprite_->Update();
}

void LightFlash::Debug_Gui(){
	if(ImGui::Button("Save")){
		JsonItems::Save("TitleScene",parameter_.ToJson("LightFlash"));
	}
	float prevDuration = parameter_.flashDuration_;
	if(ImGui::DragFloat("Duration",&parameter_.flashDuration_,0.01f)){
		// 比率を維持する
		for(auto& keyframe : parameter_.flashCurve_.keyframes){
			keyframe.time = (keyframe.time / prevDuration) * parameter_.flashDuration_;
		}
	}
	ImGui::SliderFloat("CurrentTime",&parameter_.currentFlashTime_,0.0f,parameter_.flashDuration_);
	ImGui::Text("FlashCurve");

#ifdef _DEBUG
	ImGui::EditKeyFrame(
		"FlashCurve",
		parameter_.flashCurve_,
		parameter_.flashDuration_,
		0.0f,
		nullptr);
#endif // _DEBUG
}

#pragma endregion 