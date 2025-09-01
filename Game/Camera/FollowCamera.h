#pragma once
#include "Camera.h"
#include "Input.h"
#include "WorldTransform.h"

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:// Public variable
	// コンストラクタ
	FollowCamera();
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

private:// Private method
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputUpdate();

private:// Engine system
	// 入力
	Input* input_;

private:// Private variable
	// カメラ
	Camera* pCamera_;
};