#include "StageRenderTarget.h"
#include "Engine.h"

StageRenderTarget::~StageRenderTarget() {
	for (uint32_t oi = 0; oi < stageRenderTargetNum_; ++oi) {
		renderTargetResource_[oi]->Finalize();
	}
	depthResource_->Finalize();
}

void StageRenderTarget::Init() {
	GraphicsContext* graphicsCtx = GraphicsContext::GetInstance();

	dxHeap_ = graphicsCtx->GetDxHeap();
	device_ = graphicsCtx->GetDevice();

	CreateRenderTarget();

	// heap上にDSCを構築
	D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	// 振動情報の初期化
	depthResource_ = std::make_unique<DxResource>();
	depthResource_->Init(device_, dxHeap_, ResourceType::DEPTH);
	depthResource_->CreateDepthResource(kWindowWidth_, kWindowHeight_);
	
	depthHandle_ = dxHeap_->AllocateDSV();
	device_->CreateDepthStencilView(depthResource_->GetResource(), &desc, depthHandle_.handleCPU);

}

void StageRenderTarget::SetRenderTarget(ID3D12GraphicsCommandList* commandList, uint32_t index) {
	// MRT用に複数のRTVハンドルを用意
	commandList->OMSetRenderTargets(1, &renderTargetResource_[index]->GetRTV().handleCPU, FALSE, &depthHandle_.handleCPU);
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	// RenderTargetはoffScreen用のRenderTargetを指定しておく
	// 各レンダーターゲットをクリア
	commandList->ClearRenderTargetView(renderTargetResource_[index]->GetRTV().handleCPU, clearColor, 0, nullptr);
	// 指定した深度で画面をクリア
	commandList->ClearDepthStencilView(depthHandle_.handleCPU, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	// srv
	ID3D12DescriptorHeap* descriptorHeaps[] = { dxHeap_->GetSRVHeap() };
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
}

void StageRenderTarget::Transition(ID3D12GraphicsCommandList* commandList, uint32_t index, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState) {
	renderTargetResource_[index]->Transition(commandList, beforState, afterState);
}

void StageRenderTarget::CreateRenderTarget() {
	// resourceの設定
	D3D12_RESOURCE_DESC desc{};
	desc.Width = kWindowWidth_;			// 画面の横幅
	desc.Height = kWindowHeight_;			// 画面の縦幅
	desc.MipLevels = 1;			// 
	desc.DepthOrArraySize = 1;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// 実際の初期化
	for (uint32_t oi = 0; oi < stageRenderTargetNum_; ++oi) {
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		renderTargetResource_[oi] = std::make_unique<DxResource>();
		renderTargetResource_[oi]->Init(device_, dxHeap_, ResourceType::RENDERTARGET);

		renderTargetResource_[oi]->CreateResource(&desc, &heapProperties, D3D12_HEAP_FLAG_ALLOW_DISPLAY, D3D12_RESOURCE_STATE_RENDER_TARGET);
		renderTargetResource_[oi]->CreateRTV(rtvDesc);
		renderTargetResource_[oi]->CreateSRV(srvDesc);
	}
}