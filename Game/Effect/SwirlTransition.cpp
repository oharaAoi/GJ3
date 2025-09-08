#include "SwirlTransition.h"
#include "Engine.h"
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
		float t = time_ / startData_.transitionTime;
		float kr = 0;
		float speed = 0;
		float patternAlpha = 0;
		if (!isOpen_) {
			kr = std::lerp(startData_.radiusKernel, endData_.radiusKernel, t);
			speed = std::lerp(startData_.rotateSpeed, endData_.rotateSpeed, t);
			patternAlpha = std::lerp(0.0f, 1.0f, t);
		} else {
			kr = std::lerp(endData_.radiusKernel, startData_.radiusKernel, t);
			speed = std::lerp(endData_.rotateSpeed, startData_.rotateSpeed, t);
			patternAlpha = std::lerp(1.0f, 0.0f, t);
		}

		swirl_->SetRadiusKernel(kr);
		swirl_->SetRotateSpeed(speed);
		swirl_->SetPatternAlpha(patternAlpha);

		if (time_ >= startData_.transitionTime) {
			isFinish_ = true;
			swirl_->SetIsEnable(false);
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
		if (ImGui::Button("Insert Start Data")) {
			ImGui::DragFloat("transitionTime", &startData_.transitionTime);
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
	isFinish_ = false;
	time_ = 0;
}

void SwirlTransition::Open() {
	isOpen_ = true;
	swirl_->Reset();
	swirl_->SetIsEnable(true);
	swirl_->SetRadiusKernel(startData_.radiusKernel);
	swirl_->SetRotateSpeed(startData_.rotateSpeed);
	isFinish_ = false;
	time_ = 0;
}
