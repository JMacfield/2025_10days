#pragma once
#include "Camera.h"
#include "Input.h"
#include "WorldTransform.h"
#include "../Player/Components/Anim.h"
#include "../Player/Components/Shake.h"

class Player;
/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:
	// 視野角の種類
	enum class FovType {
		kDefault,	// 通常
		kDead,		// 死亡
		kAir,		// 空中
		kLanding	// 着地の瞬間
	};

public:// Public variable
	// コンストラクタ
	FollowCamera(Player* player);
	// デストラクタ
	~FollowCamera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
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
	/// 入力処理
	/// </summary>
	void InputUpdate();

	/// <summary>
	/// 視野角の変動
	/// </summary>
	void FovUpdate();

	/// <summary>
	/// 死亡時のカメラ演出
	/// </summary>
	void DeadEffect();

public:// Accessor method
	/// <summary>
	/// カメラのアドレスを取得
	/// </summary>
	/// <returns></returns>
	Camera* GetCamera() { return camera_.get(); }

private:// 調整項目
	// 死亡時のカメラの揺れる範囲
	Vector3 deadShakeRange = { 0.4f,0.4f, 0.4f };
	float deadShakeFrame = 15.0f;

	// 初期視野角
	float defaultFov = 1.0f;
	// 空中にいるときの視野角
	float airFov = 1.1f;
	// 着地時の視野角
	float landingFov = 0.85f;
	// 死亡時の視野角
	float deadFov = 0.5f;

	// 回転の補間レート
	float rotateRate = 0.6f;

private:// Engine system
	// 入力
	Input* input_;

private:// 外部からアドレスを受け取る変数
	// 自機
	Player* player_;

private:// Private variable
	// カメラ
	std::unique_ptr<Camera> camera_;

	// 視野角のイージング
	std::map<FovType, Anim> fovAnim_;
	// 現在の視野角
	float currentFov_;

	// 死亡時の揺れ
	std::unique_ptr<Shake> deadShake_;
	Vector3 shakeOffset_;

	// 追従対象との距離
	Vector3 targetOffset_ = { 0.0f,0.0f, 0.0f };
};