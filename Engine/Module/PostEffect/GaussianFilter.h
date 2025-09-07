#pragma once
#include "Engine/Module/PostEffect/IPostEffect.h"

class GaussianFilter :
	public IPostEffect {
public:

	struct Setting {
		float deviation;
		uint32_t size;
	};

	struct GaussianFilterParam : public IJsonConverter {
		float deviation;
		uint32_t size;
		bool isEnable;

		GaussianFilterParam() { SetName("GaussianFilter"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("deviation", deviation)
				.Add("size", size)
				.Add("isEnable", isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "deviation", deviation);
			fromJson(jsonData, "size", size);
			fromJson(jsonData, "isEnable", isEnable);
		}
	};

public:

	GaussianFilter() = default;
	~GaussianFilter() override;

	void Init();

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

private:

	std::unique_ptr<DxResource> settingBuffer_;
	Setting* setting_;

	GaussianFilterParam param_;
};

