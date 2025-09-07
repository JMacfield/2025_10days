#include "Collider.h"
#include "CollisionManager.h"
#include "../Player/Components/Math/MathFuncs.h"
#include <cmath>

Collider::Collider() {
	// 衝突マネージャーのインスタンスを取得
	collisionManager_ = CollisionManager::GetInstance();
	// コライダーリスト登録
	collisionManager_->SetColliderList(this);
	// 当たり判定のワールドトランスフォームの初期化
	worldTransform.Initialize();
}

Collider::~Collider() {
	collisionManager_->ClearColliderList(this);
}

bool Collider::TriggerOnCollision() {
	if (!isPreOnCollision_ && isOnCollision_) {
		return true;
	}
	else {
		return false;
	}
}

bool Collider::ReleaseOnCollision() {
	if (isPreOnCollision_ && !isOnCollision_) {
		return true;
	}
	else {
		return false;
	}
}

bool Collider::PressOnCollision() {
	if (isOnCollision_) {
		return true;
	}
	else {
		return false;
	}
}
void Collider::SetOBBDirect(const int& index) {
    Matrix4x4 rotate = MathFuncs::MakeRotateMatrix(GetRotation());
    obb_.m_NormaDirect[index] = { rotate.m[index][0], rotate.m[index][1], rotate.m[index][2] };
}
