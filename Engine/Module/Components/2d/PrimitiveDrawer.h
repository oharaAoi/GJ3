#pragma once
#include <memory>
#include "Engine/DirectX/Utilities/DirectXUtils.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/Lib/Math/MyMatrix.h"
#include "Engine/Lib/Math/MyMath.h"
#include "Engine/System/Manager/ImGuiManager.h"

class PrimitiveDrawer {
public : // データ構造体

	struct PrimitiveData {
		Vector4 pos;
		Vector4 color;
	};

public:

	// 線分の最大数
	static const UINT kMaxLineCount = 40960;
	// 線分の頂点数
	static const UINT kVertexCountLine = 2;

	PrimitiveDrawer() {};
	~PrimitiveDrawer() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(ID3D12Device* device);

	void Finalize();

	void Draw(const Vector3& p1, const Vector3& p2, const Vector4& color, const Matrix4x4& wvpMat);

	void DrawCall(ID3D12GraphicsCommandList* commandList);

	void SetUseIndex(const uint32_t& count) { useIndex_ = count; }

	void Begin();

private:

	// 定数バッファ
	ComPtr<ID3D12Resource> vertexBuffer_;
	ComPtr<ID3D12Resource> indexBuffer_;
	ComPtr<ID3D12Resource> materialBuffer_;
	ComPtr<ID3D12Resource> wvpBuffer_;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};

	DescriptorHandles wvpSRV_;

	// データポインタ
	PrimitiveData* primitiveData_;
	uint32_t* indexData_ = nullptr;
	Vector4* materialData_;
	Matrix4x4* wvpData_;

	// 使用している線の頂点の数
	uint32_t useIndex_ = 0;

	uint32_t preUseIndex_;

};
