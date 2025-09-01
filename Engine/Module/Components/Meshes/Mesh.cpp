#include "Mesh.h"

Mesh::Mesh() {
}

Mesh::~Mesh() {
	Finalize();
}

void Mesh::Finalize() {
	vertexBuffer_.Reset();
	indexBuffer_.Reset();
}

void Mesh::Init(ID3D12Device* device, std::vector<VertexData> vertexData, std::vector<uint32_t> indices) {
	// ---------------------------------------------------------------
	// ↓Vetrtexの設定
	// ---------------------------------------------------------------
	// VertexBufferViewを作成する
	vertexBuffer_ = CreateBufferResource(device, vertexData.size() * sizeof(VertexData));
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = (UINT)vertexData.size() * sizeof(VertexData);
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	// Resourceにデータを書き込む 
	vertexData_ = nullptr;
	// アドレスを取得
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, vertexData.data(), sizeof(VertexData) * vertexData.size());
	vertexDataSize_ = static_cast<uint32_t>(vertexData.size());

	initVertexData_ = new VertexData[vertexDataSize_];
	outputVertexData_ = new VertexData[vertexDataSize_];
	for (size_t i = 0; i < vertexDataSize_; ++i) {
		initVertexData_[i] = vertexData_[i];
		outputVertexData_[i] = vertexData_[i];
	}

	verticesData_ = vertexData;
	
	// ---------------------------------------------------------------
	// ↓indexの設定
	// ---------------------------------------------------------------
	indices_ = indices;

	indexBuffer_ = CreateBufferResource(device, sizeof(uint32_t) * indices.size());
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexData_ = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, indices.data(), sizeof(uint32_t) * indices.size());

	indexNum_ = (uint32_t)indices.size();
}

void Mesh::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
}

void Mesh::DrawIndex(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetIndexBuffer(&indexBufferView_);
}

void Mesh::SetInitVertex() {
	for (size_t i = 0; i < vertexDataSize_; ++i) {
		vertexData_[i] = initVertexData_[i];
	}
}

void Mesh::SetOutputVertexData(VertexData* vertexData) {
	for (size_t i = 0; i < vertexDataSize_; ++i) {
		outputVertexData_[i] = vertexData[i];
	}
}

void Mesh::SetVertexData(const std::vector<VertexData>& vertexData) {
	if (vertexDataSize_ <= vertexData.size()) {
		verticesData_ = vertexData;
	}
}

void Mesh::SetIndices(const std::vector<uint32_t>& indices) {
	if (indexNum_ <= indices.size()) {
		indices_ = indices;
	}
}
