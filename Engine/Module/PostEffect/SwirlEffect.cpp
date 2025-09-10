#include "SwirlEffect.h"
#include "Engine.h"
#include "Engine/Core/GraphicsContext.h"
#include "Engine/System/Manager/TextureManager.h"

SwirlEffect::~SwirlEffect() {
	settingBuffer_->Finalize();
}

void SwirlEffect::Init() {
	GraphicsContext* graphicsCtx = GraphicsContext::GetInstance();
	settingBuffer_ = std::make_unique<DxResource>();
	settingBuffer_->Init(graphicsCtx->GetDevice(), graphicsCtx->GetDxHeap(), ResourceType::COMMON);
	settingBuffer_->CreateResource(sizeof(SwirlSetting));
	settingBuffer_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&setting_));

	// resourceの設定
	D3D12_RESOURCE_DESC desc{};
	desc.Width = kWindowWidth_;			// 画面の横幅
	desc.Height = kWindowHeight_;			// 画面の縦幅
	desc.MipLevels = 1;			// 
	desc.DepthOrArraySize = 1;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	sceneBuffer_ = std::make_unique<DxResource>();
	sceneBuffer_->Init(graphicsCtx->GetDevice(), graphicsCtx->GetDxHeap(), ResourceType::COMMON);
	sceneBuffer_->CreateResource(&desc, &heapProperties, D3D12_HEAP_FLAG_ALLOW_DISPLAY, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	sceneBuffer_->CreateSRV(srvDesc);

	setting_->screenSize = { kWindowWidth_, kWindowHeight_ };
	setting_->time = 0;
	setting_->frontWidth = 1;
	param_.frontWidth = 1;

	swirlMask_ = std::make_unique<SwirlMask>();
	swirlMask_->Init();

	isEnable_ = true;
}

void SwirlEffect::SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) {
	pingResource->Transition(commandList, D3D12_RESOURCE_STATE_COPY_SOURCE);
	sceneBuffer_->Transition(commandList, D3D12_RESOURCE_STATE_COPY_DEST);
	commandList->CopyResource(sceneBuffer_->GetResource(), pingResource->GetResource());
	// copyの状態から戻す
	pingResource->Transition(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	sceneBuffer_->Transition(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	swirlMask_->SetCommand();

	postProcessResource_->Swap(commandList);
	postProcessResource_->SetRenderTarget(commandList, BufferType::PONG, depthHandle_.handleCPU);
	
	CopyData();
	//setting_->uv = param_.uvTransform.MakeAffine()
	setting_->time += GameTimer::DeltaTime() * param_.rotateSpeed;

	Pipeline* pso = Engine::SetPipeline(PSOType::ProcessedScene, "PostProcess_Swirl.json");
	UINT index = pso->GetRootSignatureIndex("gSceneTexture");
	commandList->SetGraphicsRootDescriptorTable(index, sceneBuffer_->GetSRV().handleGPU);
	index = pso->GetRootSignatureIndex("gPatternTexture");
	commandList->SetGraphicsRootDescriptorTable(index, postProcessResource_->GetPingResource()->GetSRV().handleGPU);
	index = pso->GetRootSignatureIndex("gSetting");
	commandList->SetGraphicsRootConstantBufferView(index, settingBuffer_->GetResource()->GetGPUVirtualAddress());
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

void SwirlEffect::SetShaderResourceCommand(ID3D12GraphicsCommandList* commandList, ShaderResource* pingResource) {
	pingResource->Transition(commandList, D3D12_RESOURCE_STATE_COPY_SOURCE);
	sceneBuffer_->Transition(commandList, D3D12_RESOURCE_STATE_COPY_DEST);
	commandList->CopyResource(sceneBuffer_->GetResource(), pingResource->GetResource());
	// copyの状態から戻す
	pingResource->Transition(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	sceneBuffer_->Transition(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	swirlMask_->SetCommand();

	postProcessResource_->Swap(commandList);
	postProcessResource_->SetRenderTarget(commandList, BufferType::PONG, depthHandle_.handleCPU);

	CopyData();
	//setting_->uv = param_.uvTransform.MakeAffine()
	setting_->time += GameTimer::DeltaTime() * param_.rotateSpeed;

	Pipeline* pso = Engine::SetPipeline(PSOType::ProcessedScene, "PostProcess_Swirl.json");
	UINT index = pso->GetRootSignatureIndex("gSceneTexture");
	commandList->SetGraphicsRootDescriptorTable(index, sceneBuffer_->GetSRV().handleGPU);
	index = pso->GetRootSignatureIndex("gPatternTexture");
	commandList->SetGraphicsRootDescriptorTable(index, postProcessResource_->GetPingResource()->GetSRV().handleGPU);
	index = pso->GetRootSignatureIndex("gSetting");
	commandList->SetGraphicsRootConstantBufferView(index, settingBuffer_->GetResource()->GetGPUVirtualAddress());
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

void SwirlEffect::CheckBox() {
	ImGui::Checkbox("SwirlEffect##SwirlEffectSwirlEffect_checkBox", &isEnable_);
}

void SwirlEffect::Debug_Gui() {
	if (ImGui::CollapsingHeader("SwirlEffect##SwirlEffect")) {
		ImGui::DragFloat("time", &setting_->time, 0.01f);
		ImGui::DragFloat("radiusKernel", &param_.radiusKernel, 0.01f);
		ImGui::DragFloat("angleStrength", &param_.angleStrength, 0.01f);
		ImGui::DragFloat("speed", &param_.speed, 0.01f);
		ImGui::DragFloat("frontWidth", &param_.frontWidth, 0.01f);
		ImGui::DragFloat("swirlStrength", &param_.swirlStrength, 0.01f);
		ImGui::DragFloat("fineness", &param_.fineness, 0.01f);
		ImGui::DragFloat("thickness", &param_.thickness, 0.01f);

		ImGui::DragFloat("rotateSpeed", &param_.rotateSpeed, 0.01f);

		swirlMask_->Debug_Gui();

		/*if (ImGui::Button("Save")) {
			param_.isEnable = isEnable_;
			JsonItems::Save("PostEffect", param_.ToJson(param_.GetName()));
		}
		if (ImGui::Button("Apply")) {
			param_.FromJson(JsonItems::GetData("PostEffect", param_.GetName()));
		}*/
	}
}

void SwirlEffect::ApplySaveData() {
}

void SwirlEffect::CopyData() {
	setting_->radiusKernel = param_.radiusKernel;
	setting_->angleStrength = param_.angleStrength;
	setting_->speed = param_.speed;
	setting_->frontWidth = param_.frontWidth;
	setting_->swirlStrength = param_.swirlStrength;
	setting_->fineness = param_.fineness;
	setting_->thickness = param_.thickness;
}

void SwirlEffect::Reset() {
	setting_->time = 0;
}
