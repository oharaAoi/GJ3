#pragma once
#include <optional>
#include "Engine/DirectX/Utilities/DirectXUtils.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"

enum class ResourceType {
	COMMON,
	RENDERTARGET,
	DEPTH
};

class DxResource {
public:

	DxResource();
	~DxResource();

	void Finalize();

	void Init(ID3D12Device* device, DescriptorHeap* dxHeap, ResourceType type);

	void CreateResource(const size_t& size);
	void CreateResource(D3D12_RESOURCE_DESC* resourceDesc, D3D12_HEAP_PROPERTIES* heapProperties,
							const D3D12_HEAP_FLAGS& heapFlags, const D3D12_RESOURCE_STATES& resourceState);
	void CreateDepthResource(uint32_t width, uint32_t height);

	void CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc);
	void CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc);
	void CreateRTV(const D3D12_RENDER_TARGET_VIEW_DESC& desc);

	void SetSwapChainBuffer(IDXGISwapChain4* swapChain, uint32_t indexn);

	void Transition(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
	void Transition(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& after);

public:

	ID3D12Resource* GetResource() { return cBuffer_.Get(); }
	ComPtr<ID3D12Resource> GetCompResource() { return cBuffer_; }

	const D3D12_RESOURCE_STATES& GetState() const { return bufferState_; }

	const DescriptorHandles& GetSRV() const;
	const DescriptorHandles& GetUAV() const;
	const DescriptorHandles& GetRTV() const;

private:

	ID3D12Device* pDevice_ = nullptr;
	DescriptorHeap* pDxHeap_ = nullptr;

	ComPtr<ID3D12Resource> cBuffer_;
	D3D12_RESOURCE_STATES bufferState_;

	std::optional<DescriptorHandles> uavAddress_ = std::nullopt;
	std::optional<DescriptorHandles> srvAddress_ = std::nullopt;
	std::optional<DescriptorHandles> rtvAddress_ = std::nullopt;

	ResourceType type_;
};

