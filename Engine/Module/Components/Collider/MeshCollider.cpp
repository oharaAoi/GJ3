#include "MeshCollider.h"

MeshCollider::MeshCollider() {}
MeshCollider::~MeshCollider() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void MeshCollider::Init(Mesh* mesh) {
	mesh_ = mesh;

	size_ = CVector3::ZERO;
	maxSize_ = { -9999.0f, -9999.0f, -9999.0f };
	minSize_ = { 9999.0f, 9999.0f, 9999.0f };

	collisionState_ = 0b00;

	// meshのVertexから各軸での最大の値を取り出す
	VertexData* vertices = mesh_->GetOutputVertexData();
	for (uint32_t index = 0; index < mesh_->GetVertexSize(); ++index) {
		//Vector3 size = Transform(Vector3(vertices[index].pos.x, vertices[index].pos.y , vertices[index].pos.z), worldMat);
		Vector3 size = Vector3(vertices[index].pos.x, vertices[index].pos.y, vertices[index].pos.z);

		// -------------------------------------------------
		// ↓ 最小を取得
		// -------------------------------------------------
		if (size.x < minSize_.x) {
			minSize_.x = size.x;
		}

		if (size.y < minSize_.y) {
			minSize_.y = size.y;
		}

		if (size.z < minSize_.z) {
			minSize_.z = size.z;
		}

		// -------------------------------------------------
		// ↓ 最大を取得
		// -------------------------------------------------
		if (size.x > maxSize_.x) {
			maxSize_.x = size.x;
		}

		if (size.y > maxSize_.y) {
			maxSize_.y = size.y;
		}

		if (size.z > maxSize_.z) {
			maxSize_.z = size.z;
		}
	}

	// -------------------------------------------------
	// ↓ 最大値と最小の値から物体の半径を求める
	// -------------------------------------------------
	float xRadius = std::abs(maxSize_.x - minSize_.x) * 0.5f;
	float yRadius = std::abs(maxSize_.y - minSize_.y) * 0.5f;
	float zRadius = std::abs(maxSize_.z - minSize_.z) * 0.5f;

	size_ = { xRadius, yRadius, zRadius };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MeshCollider::Update(const WorldTransform* worldTransform, const Vector3& offset) {
	maxSize_ = { -9999.0f, -9999.0f, -9999.0f };
	minSize_ = { 9999.0f, 9999.0f, 9999.0f };

	// meshのVertexから各軸での最大の値を取り出す
	VertexData* vertices = mesh_->GetOutputVertexData();
	for (uint32_t index = 0; index < mesh_->GetVertexSize(); ++index) {
		Vector3 size = Vector3(vertices[index].pos.x, vertices[index].pos.y , vertices[index].pos.z);
		//Vector3 size = Transform(Vector3(vertices[index].pos.x, vertices[index].pos.y, vertices[index].pos.z), worldTransform->GetWorldMatrix());

		// -------------------------------------------------
		// ↓ 最小を取得
		// -------------------------------------------------
		if (size.x < minSize_.x) {
			minSize_.x = size.x;
		}

		if (size.y < minSize_.y) {
			minSize_.y = size.y;
		}

		if (size.z < minSize_.z) {
			minSize_.z = size.z;
		}

		// -------------------------------------------------
		// ↓ 最大を取得
		// -------------------------------------------------
		if (size.x > maxSize_.x) {
			maxSize_.x = size.x;
		}

		if (size.y > maxSize_.y) {
			maxSize_.y = size.y;
		}

		if (size.z > maxSize_.z) {
			maxSize_.z = size.z;
		}
	}

	// -------------------------------------------------
	// ↓ 最大値と最小の値から物体の半径を求める
	// -------------------------------------------------
	float xRadius = std::abs(maxSize_.x - minSize_.x) * 0.5f;
	float yRadius = std::abs(maxSize_.y - minSize_.y) * 0.5f;
	float zRadius = std::abs(maxSize_.z - minSize_.z) * 0.5f;
	size_ = { xRadius, yRadius, zRadius };
	
	if (xRadius < yRadius) {
		radius_ = yRadius;
	}

	if (radius_ < zRadius) {
		radius_ = zRadius;
	}

	// -------------------------------------------------
	// ↓ OBBの更新
	// -------------------------------------------------
	obb_.size = size_;
	obb_.center = (maxSize_ + minSize_) * 0.5f;
	obb_.center = Transform(obb_.center + offset, worldTransform->GetWorldMatrix());
	obb_.MakeOBBAxis(worldTransform->GetRotate());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void MeshCollider::Draw() const {
	DrawOBB(obb_, Render::GetViewport3D() * Render::GetProjection3D(), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　状態の変更
//////////////////////////////////////////////////////////////////////////////////////////////////

void MeshCollider::SwitchCollision() {
	switch (collisionState_) {
	case 0b00:	// 衝突していない
		collisionState_ = CollisionFlags::ENTER;	// NONE → ENTER
		break;
	case 0b01:	// 初衝突
		collisionState_ = CollisionFlags::STAY;		// ENTER → STAY
		break;
	case 0b10:	// 衝突しなくなった直後
		collisionState_ = CollisionFlags::NONE;		// EXIT → NONE
		break;
	case 0b11:	// 連続衝突時
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　collision判定
//////////////////////////////////////////////////////////////////////////////////////////////////

void MeshCollider::OnCollision(MeshCollider& other) {
	switch (collisionState_) {
	case 0b00:	// 衝突していない
		break;
	case 0b01:	// 初衝突
		OnCollisionEnter(other);
		break;
	case 0b10:	// 衝突しなくなった直後
		OnCollisionExit(other);
		break;
	case 0b11:	// 連続衝突時
		OnCollisionStay(other);
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　collisionの内容
//////////////////////////////////////////////////////////////////////////////////////////////////

void MeshCollider::OnCollisionEnter(MeshCollider& other) {
	if (onCollisionEnter_) {
		onCollisionEnter_(other);
	}
}

void MeshCollider::OnCollisionStay(MeshCollider& other) {
	if (onCollisionStay_) {
		onCollisionStay_(other);
	}
}

void MeshCollider::OnCollisionExit(MeshCollider& other) {
	if (onCollisionExit_) {
		onCollisionExit_(other);
	}
}
