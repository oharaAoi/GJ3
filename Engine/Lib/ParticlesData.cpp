#include "ParticlesData.h"
#include "imgui.h"

void ParticleEmit::Attribute_Gui() {
	if (ImGui::CollapsingHeader("Emitter Settings")) {
		ImGui::Checkbox("IsLoop", &isLoop);
		ImGui::DragFloat("Duration", &duration);
		ImGui::Checkbox("isBillBord", &isBillBord);
		ImGui::BulletText("Emitter Settings");
		ImGui::DragFloat3("Rotate", &rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", (float*)&translate, 0.1f);
		ImGui::DragScalar("Shape", ImGuiDataType_U32, &shape, 1.0f);
		ImGui::DragScalar("RateOverTimeCout", ImGuiDataType_U32, &rateOverTimeCout, 1.0f);
		
		ImGui::Combo("Shape##", &shape, "SPHERE\0BOX\0CONE");
		if (shape == (int)CpuEmitterShape::SPHERE) {
			ImGui::DragFloat("radius", &radius, 0.1f);
		} else if (shape == (int)CpuEmitterShape::BOX) {
			ImGui::DragFloat3("size", &size.x, 0.1f);
		} else if (shape == (int)CpuEmitterShape::CONE) {
			ImGui::DragFloat("radius", &radius, 0.1f);
			ImGui::DragFloat("angle", &angle, 0.1f);
			ImGui::DragFloat("height", &height, 0.1f);
		}
		ImGui::Combo("emitOrigin##emitOrigin", &emitOrigin, "CENTER\0RANGE\0");
		ImGui::Combo("emitDirection##type", &emitDirection, "UP\0ALLDIRE\0OUTSIDE\0CENTERFOR");
		ImGui::Separator();

		ImGui::BulletText("Particle Parameters");
		ImGui::Checkbox("RandomColor", &isRandomColor);
		if (isRandomColor) {
			ImGui::ColorEdit4("randColor1", (float*)&randColor1);
			ImGui::ColorEdit4("randColor2", (float*)&randColor2);
		} else {
			ImGui::ColorEdit4("Color", (float*)&color); 
		}
		ImGui::Checkbox("isLerpDiscard", &isLerpDiscardValue);
		if (!isLerpDiscardValue) {
			ImGui::DragFloat("discardValue", &discardValue, 0.01f, 0.0f, 1.0f);
		} else {
			ImGui::DragFloat("startDiscard", &startDiscard, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("endDiscard", &endDiscard, 0.01f, 0.0f, 1.0f);
		}
		ImGui::Checkbox("SeparateByAxisScale", &separateByAxisScale);
		if (separateByAxisScale) {
			ImGui::DragFloat3("minScale", &minScale.x, 0.1f);
			ImGui::DragFloat3("maxScale", &maxScale.x, 0.1f);
		} else {
			ImGui::DragFloat("scaleMinScaler", &minScale.x);
			ImGui::DragFloat("scaleMaxScaler", &maxScale.x);
		}
		ImGui::Text("Parameter");
		ImGui::DragFloat("Speed", &speed, 0.01f);
		ImGui::DragFloat("Lifetime", &lifeTime, 0.01f);
		ImGui::DragFloat("Gravity", &gravity, 0.01f);
		ImGui::DragFloat("Damping", &dampig, 0.01f);
		ImGui::Text("First Angle");
		ImGui::DragFloat("angleMin", &angleMin, 0.01f);
		ImGui::DragFloat("angleMax", &angleMax, 0.01f);
		ImGui::Text("Life of");
		ImGui::Checkbox("isDirectionRotate", &isDirectionRotate);
		ImGui::Checkbox("isLifeOfScale", &isLifeOfScale);
		ImGui::Checkbox("isLifeOfAlpha", &isLifeOfAlpha);
		ImGui::Checkbox("FadeInOut", &isFadeInOut);
		if (isFadeInOut) {
			ImGui::DragFloat("fadeInTime", &fadeInTime, 0.1f);
			ImGui::DragFloat("fadeOutTime", &fadeOutTime, 0.1f);
		}

		ImGui::Checkbox("isParticleAddBlend", &isParticleAddBlend);
		ImGui::Checkbox("isDraw2d", &isDraw2d);
		ImGui::Checkbox("isScaleUp", &isScaleUp);
		ImGui::DragFloat3("scaleUpScale", (float*)&scaleUpScale, 0.01f);

		minScale.Clamp(minScale,maxScale );
	}
}