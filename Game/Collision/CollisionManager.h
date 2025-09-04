#pragma once
#include "CollisionConfig.h"
#include "Collider.h"
#include <list>

/// <summary>
/// 衝突を検知するクラス
/// </summary>
class CollisionManager {
public:
	~CollisionManager();

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static CollisionManager* GetInstance();

	/// <summary>
	/// 当たっているかの確認
	/// </summary>
	void CheckAllCollisions();

private:// Private mathod
	/// <summary>
	/// 当たっているなら衝突応答関数を呼ぶ
	/// </summary>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 当たっているかの状態を初期化
	/// </summary>
	void ResetIsOnCollision();

	/// <summary>
	/// OBB同士の衝突判定
	/// </summary>
	/// <param name="obb1"></param>
	/// <param name="obb2"></param>
	/// <returns></returns>
	bool ColOBBs(const OBB& obb1, const OBB& obb2);

	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);

public:// Accessor method
	/// <summary>
	/// コライダーリストをすべてクリア
	/// </summary>
	void ClearColliderList() {
		colliders_.clear();
	}
	/// <summary>
	/// 特定のコライダーをクリア
	/// </summary>
	/// <param name="collider"></param>
	void ClearColliderList(Collider* collider) {
		colliders_.remove(collider);
	}

	///
	/// Setter
	/// 

	/// <summary>
	/// コライダーリストに登録
	/// </summary>
	/// <param name="collider">当たり判定をつけたいオブジェクト</param>
	void SetColliderList(Collider* collider) {
		colliders_.push_back(collider);
	}

private:
	// コライダーリスト
	std::list<Collider*> colliders_;
};