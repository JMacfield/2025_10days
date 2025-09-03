#pragma once
#include "Camera.h"
#include "Input.h"
#include "WorldTransform.h"

class Player;
/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
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
	/// 入力処理
	/// </summary>
	void InputUpdate();

public:// Accessor method
	/// <summary>
	/// カメラのアドレスを取得
	/// </summary>
	/// <returns></returns>
	Camera* GetCamera() { return camera_.get(); }

private:// Engine system
	// 入力
	Input* input_;

private:// 外部からアドレスを受け取る変数
	// 自機
	Player* player_;

private:// Private variable
	// カメラ
	std::unique_ptr<Camera> camera_;

	Vector3 targetOffset_ = { 0.0f,0.0f, 0.0f };

	// 角度
	Vector3 rotate_;
};