#pragma once
#include "Engine/Module/PostEffect/IPostEffect.h"

class Smoothing :
	public IPostEffect {
public:

	struct Setting {
		uint32_t size;
	};

	struct SmoothingParam : public IJsonConverter {
		uint32_t size;
		bool isEnable;

		SmoothingParam() { SetName("Smoothing"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("size", size)
				.Add("isEnable", isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "size", size);
			fromJson(jsonData, "isEnable", isEnable);
		}
	};

public:

	Smoothing() = default;
	~Smoothing() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

private:

	std::unique_ptr<DxResource> settingBuffer_;
	Setting* setting_;

	SmoothingParam param_;

};

