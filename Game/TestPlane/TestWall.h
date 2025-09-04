#pragma once
#include "Object3D.h"
#include "Collider.h"

class TestWall {
public:// Public variable
	// コンストラクタ
	TestWall();
	// デストラクタ
	~TestWall() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

private:// Private method
	/// <summary>
	/// 衝突応答
	/// </summary>
	/// <param name="collider"></param>
	void OnCollision(Collider* collider);

public:// Accessor method
	/// <summary>
	/// カメラのアドレスを設定
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { pCamera_ = camera; }
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="pos"></param>
	void SetTranslation(const Vector3& pos) { body_->worldTransform_.translation_ = pos; }
	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(const Vector3& rot) { body_->worldTransform_.rotation_ = rot; }

private:// 外部から受け取るアドレス
	// カメラ
	Camera* pCamera_;

private:// Private variable
	// 体のオブジェクト
	std::unique_ptr<Object3d> body_;

	// 当たり判定
	std::unique_ptr<Collider> collider_;

	// テクスチャ
	uint32_t texHandle_;
};

