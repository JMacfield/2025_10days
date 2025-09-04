#pragma once
#include "Vector3.h"

/// <summary>
/// 移動処理
/// </summary>
class MoveSystem {
public:
	// コンストラクタ
	MoveSystem() = default;
	// デストラクタ
	~MoveSystem() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

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
#pragma region Getter
	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetVel() { return vel_; }
	/// <summary>
	/// 角度を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetRot() { return rot_; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:// 調整項目
	Vector3 maxMoveVel = { 0.0f,0.0f, 0.05f };

private:// Private method
	// 速度
	Vector3 vel_;

	Vector3 rot_;
};