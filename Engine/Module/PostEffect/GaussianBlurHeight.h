#pragma once
#include "Engine/Module/PostEffect/IPostEffect.h"
#include "Engine/Lib/Math/Vector2.h"

class GaussianBlurHeight :
	public IPostEffect {
public:	// 構造体

	struct BlurSettings {
		Vector2 texelSize;
	};

	struct GaussianBlurHeightParam : public IJsonConverter {
		Vector2 texelSize = {1,1};

		GaussianBlurHeightParam() { SetName("GaussianBlurHeight"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("texelSize", texelSize)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "texelSize", texelSize);
		}
	};

public:

	GaussianBlurHeight() = default;
	~GaussianBlurHeight() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

private:

	std::unique_ptr<DxResource> blurBuffer_;
	BlurSettings* blurSetting_;

	GaussianBlurHeightParam param_;
public:
	Vector2 GetTexelSize() const{ return param_.texelSize; }
	void SetTexelSize(const Vector2& size){ param_.texelSize = size; }
	void SetTexelSizeFromFloat(const float& size){ param_.texelSize = Vector2(size,size); }
};

