#include "SwirlTransition.h"
#include "Engine.h"
#include "Engine/Lib/Math/Easing.h"
#include "Engine/System/Editer/Window/EditorWindows.h"

void SwirlTransition::Init() {
	SetName("SwirlTransition");
	swirl_ = Engine::GetPostProcess()->GetSwirlEffect();
	swirl_->SetIsEnable(false);

	startData_.FromJson(JsonItems::GetData("SwirlTransition", "StartData"));
	endData_.FromJson(JsonItems::GetData("SwirlTransition", "EndData"));

	isFinish_ = true;
	isOpen_ = true;
	time_ = 0;

	EditorWindows::AddObjectWindow(this, GetName());
}

void SwirlTransition::Update() {
	if (!isFinish_) {
		time_ += GameTimer::DeltaTime();
		float kr = 0;
		float speed = 0;
		float patternAlpha = 0;
		if (!isOpen_) {
			float t = time_ / startData_.transitionTime;
			kr = std::lerp(startData_.radiusKernel, endData_.radiusKernel, Ease::Out::Quart(t));
			speed = std::lerp(startData_.rotateSpeed, endData_.rotateSpeed, Ease::Out::Quart(t));
			patternAlpha = std::lerp(0.0f, 1.0f, Ease::Out::Quart(t));
		} else {
			float t = time_ / endData_.transitionTime;
			kr = std::lerp(endData_.radiusKernel, startData_.radiusKernel, Ease::In::Cubic(t));
			speed = std::lerp(endData_.rotateSpeed, startData_.rotateSpeed, Ease::In::Cubic(t));
			patternAlpha = std::lerp(1.0f, 0.0f, Ease::In::Cubic(t));
		}

		swirl_->SetRadiusKernel(kr);
		swirl_->SetRotateSpeed(speed);
		swirl_->SetPatternAlpha(patternAlpha);

		float finishTime;
		if (!isOpen_) {
			finishTime = startData_.transitionTime;
		} else {
			finishTime = endData_.transitionTime;
		}
		if (time_ >= finishTime) {
			isFinish_ = true;

			if(isOpen_){
				swirl_->SetIsEnable(true); swirl_->SetIsEnable(false);
			}
		}
	}
}

void SwirlTransition::Debug_Gui() {
	if (ImGui::Button("Open")) {
		Open();
	}

	if (ImGui::Button("Close")) {
		Close();
	}

	if (ImGui::CollapsingHeader("Start")) {
		ImGui::DragFloat("transitionTime", &startData_.transitionTime);
		if (ImGui::Button("Insert Start Data")) {
			startData_.radiusKernel = swirl_->GetRadiusKernel();
			startData_.rotateSpeed = swirl_->GetRotateSpeed();
		}

		if (ImGui::Button("Save Start")) {
			JsonItems::Save("SwirlTransition", startData_.ToJson("StartData"));
		}
		if (ImGui::Button("Apply Start")) {
			startData_.FromJson(JsonItems::GetData("SwirlTransition", "StartData"));
		}
	}

	if (ImGui::CollapsingHeader("End")) {
		ImGui::DragFloat("transitionTime", &endData_.transitionTime);
		if (ImGui::Button("Insert End Data")) {
			endData_.radiusKernel = swirl_->GetRadiusKernel();
			endData_.rotateSpeed = swirl_->GetRotateSpeed();
		}

		if (ImGui::Button("Save End")) {
			JsonItems::Save("SwirlTransition", endData_.ToJson("EndData"));
		}
		if (ImGui::Button("Apply End")) {
			endData_.FromJson(JsonItems::GetData("SwirlTransition", "EndData"));
		}
	}
}

void SwirlTransition::Close() {
	isOpen_ = false;
	swirl_->Reset();
	swirl_->SetIsEnable(true);
	swirl_->SetRadiusKernel(startData_.radiusKernel);
	swirl_->SetRotateSpeed(startData_.rotateSpeed);
	swirl_->SetPatternAlpha(0.f);
	isFinish_ = false;
	time_ = 0;
}

void SwirlTransition::Open() {
	isOpen_ = true;
	//swirl_->Reset();
	swirl_->SetIsEnable(true);
	swirl_->SetRadiusKernel(startData_.radiusKernel);
	swirl_->SetRotateSpeed(startData_.rotateSpeed);
	isFinish_ = false;
	time_ = 0;
	swirl_->SetPatternAlpha(1.f);
}
