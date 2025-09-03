#pragma once
#include <vector>
#include "Engine/Utilities/Convert.h"
#include "Engine/DirectX/Utilities/DirectXUtils.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/Resource/DxResource.h"

static const uint32_t stageRenderTargetNum_ = 3;

class StageRenderTarget {
public:

	StageRenderTarget() = default;
	~StageRenderTarget();

	void Init();

	void SetRenderTarget(ID3D12GraphicsCommandList* commandList, uint32_t index);

	void Transition(ID3D12GraphicsCommandList* commandList, uint32_t index, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState);

	DxResource* GetResource(uint32_t index) { return renderTargetResource_[index].get(); }

private:

	void CreateRenderTarget();

private:

	std::unique_ptr<DxResource> renderTargetResource_[stageRenderTargetNum_];
	std::unique_ptr<DxResource> depthResource_;
	DescriptorHandles depthHandle_;

	ID3D12Device* device_ = nullptr;
	// heap
	DescriptorHeap* dxHeap_ = nullptr;
	// swapChain
	IDXGISwapChain4* swapChain_ = nullptr;
};

