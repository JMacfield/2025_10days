#pragma once
#include "Input.h"
#include "Object3D.h"
#include "Collider.h"
#include "Systems/MoveSystem.h"
#include "Systems/JumpSystem.h"
#include "PlayerConfig.h"

class Player {
public:// Public variable
	// コンストラクタ
	Player();
	// デストラクタ
	~Player() = default;

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
	/// テクスチャの読み込み
	/// </summary>
	void LoadTextures();

	/// <summary>
	/// 全ての機能を生成
	/// </summary>
	void CreateSystems();

	/// <summary>
	/// 衝突応答
	/// </summary>
	/// <param name="collider"></param>
	void OnCollision(Collider* collider);
	/// <summary>
	/// 着地判定
	/// </summary>
	void CheckLanding(Collider* collider);
	/// <summary>
	/// 着地時の体の角度(カメラの角度にもなる)
	/// </summary>
	void LandingRotate();

public:// Accessor method
	/// <summary>
	/// ジャンプ開始
	/// </summary>
	void StartJump();
	/// <summary>
	/// ジャンプ終了
	/// </summary>
	void EndJump();

#pragma region Getter
	/// <summary>
	/// 当たり判定を取得
	/// </summary>
	/// <returns></returns>
	Collider* GetCollider() { return collider_.get(); }
	/// <summary>
	/// 体のWorldTFのアドレスを取得
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &body_->worldTransform_; }
	/// <summary>
	/// 自機のどちら側にいるかを取得
	/// </summary>
	/// <returns></returns>
	WallSide GetCurrentWallSide() { return currentWallSide_; }
	/// <summary>
	/// 今の次元(過去か現在か)を取得
	/// </summary>
	/// <returns></returns>
	DimensionType GetCurrentDimension() { return currentDimension_; }
	/// <summary>
	/// 重力加速度を取得
	/// </summary>
	/// <returns></returns>
	float GetAcceleration() { return acceleration; }
	/// <summary>
	/// 空中にいるかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAir() { return isAir_; }
	/// <summary>
	/// 空中に浮いた瞬間を取得
	/// </summary>
	/// <returns></returns>
	bool GetTriggerAir() {
		if (!isPreAir_ && isAir_) {
			return true;
		}
		return false;
	}
	/// <summary>
	/// 着地した瞬間を取得
	/// </summary>
	/// <returns></returns>
	bool GetTriggerLanding() {
		if (isPreAir_ && !isAir_) {
			return true;
		}
		return false;
	}
	/// <summary>
	/// 着地しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsLanding() { return isLanding_; }
	/// <summary>
	/// 線蔵しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAlive() { return isAlive_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// カメラのアドレスを設定
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { pCamera_ = camera; }
	/// <summary>
	/// 空中にいるかを設定
	/// </summary>
	/// <param name="isAir"></param>
	void SetIsAir(const bool& isAir) { isAir_ = isAir; }
#pragma endregion

private:// 定数
	// 着地時の体の角度
	Vector3 landingRot = { 0.0f, 0.0f, 0.3f };
	// 自機が壁に着地したときの座標の補間量
	float landingOffsetX = 0.01f;

	// 重力加速度
	float acceleration = 0.05f;

private:// Engine system
	// 入力
	Input* input_;
	XINPUT_STATE joyState;
	XINPUT_STATE preJoyState;

private:// 外部から受け取るアドレス
	// カメラ
	Camera* pCamera_;

private:// Private variable
	// 体のオブジェクト
	std::unique_ptr<Object3d> body_;

	std::unique_ptr<Collider> collider_;

	// 移動処理
	std::unique_ptr<MoveSystem> moveSystem_;
	// ジャンプ処理
	std::unique_ptr<JumpSystem> jumpSystem_;

	// 現在壁の右側化左側にいるか
	WallSide currentWallSide_;

	// 速度
	Vector3 vel_;
	// 角度
	Vector3 rot_;

	// 今の次元(過去か現在か)
	DimensionType currentDimension_;

	// 空中にいるか
	bool isAir_ = true;
	bool isPreAir_ = false;
	// 着地したか
	bool isLanding_ = false;

	bool isAlive_ = true;
};

