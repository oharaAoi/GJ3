#pragma once
#include <memory>
#include "Engine/Module/PostEffect/SwirlEffect.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Lib/Json/IJsonConverter.h"

/// <summary>
/// 渦巻き演出のクラス
/// </summary>
class SwirlTransition :
	public AttributeGui {
public:

	struct TransitionData : public IJsonConverter {
		float rotateSpeed;
		float radiusKernel;
		float transitionTime = 3;

		TransitionData() { SetName("SwirlTransitionData"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("rotateSpeed", rotateSpeed)
				.Add("radiusKernel", radiusKernel)
				.Add("transitionTime", transitionTime)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "rotateSpeed", rotateSpeed);
			fromJson(jsonData, "radiusKernel", radiusKernel);
			fromJson(jsonData, "transitionTime", transitionTime);
		}
	};

public:

	SwirlTransition() = default;
	~SwirlTransition() = default;

	void Init();

	void Update();

	void Debug_Gui() override;

	void Close();
	void Open();

public:

	bool GetIsFinish() const { return isFinish_; }

private:

	std::shared_ptr<SwirlEffect> swirl_;

	TransitionData startData_;
	TransitionData endData_;

	float time_;
	bool isFinish_;
	bool isOpen_;

};

