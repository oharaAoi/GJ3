#pragma once
#include <string>
#include <memory>
#include "Engine/Module/PostEffect/IPostEffect.h"
#include "Engine/Lib/Math/MathStructures.h"
#include "Engine/Module/PostEffect/PingPongBuffer.h"
#include "Engine/Module/PostEffect/SwirlMask.h"

/// <summary>
/// 渦巻き演出
/// </summary>
class SwirlEffect :
	public IPostEffect {
public:

	struct SwirlSetting {
		Matrix4x4 uv;
	};

	struct SwirlParameter {
		SRT uvTransform;
		std::string patternTextureName;
		bool isEnable;
	};

public:

	SwirlEffect() = default;
	~SwirlEffect() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

public:

	void SetPongResource(PingPongBuffer* _resource) { postProcessResource_ = _resource; }

	void SetDepthHandle(const DescriptorHandles& _handle) { depthHandle_ = _handle; };

private:

	std::unique_ptr<SwirlMask> swirlMask_;

	std::unique_ptr<DxResource> settingBuffer_;
	SwirlSetting* setting_;

	SwirlParameter param_;

	PingPongBuffer* postProcessResource_;
	DescriptorHandles depthHandle_;

};

