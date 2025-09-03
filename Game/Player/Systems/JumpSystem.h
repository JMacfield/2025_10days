#pragma once
#include "ISystem.h"
#include "Input.h"
#include "Vector3.h"

class Player;
/// <summary>
/// ジャンプ処理
/// </summary>
class JumpSystem : public ISystem {
public:
	// コンストラクタ
	JumpSystem(Player* player);
	// デストラクタ
	~JumpSystem() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

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
	/// どちらにジャンプするかを決める
	/// </summary>
	void JumpSideUpdate();

public:// Accessor method
#pragma region Getter
	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetVel() { return vel_; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:
	// 初速
	Vector3 firstVel = {0.8f,0.5f,0.0f};

	// 重力加速度
	//float acceleration_ = 0.05f;

	// 空中でジャンプできる回数
	int maxAirJump = 1;

private:// Engine variable
	Input* input_;

private:// 外部から受け取る変数
	// 自機
	Player* player_;

private:// Private variable
	// 速度
	Vector3 vel_;

	// ジャンプ方向
	float jumpDirX_;

	// 空中でジャンプした回数
	int airJumpCount_;
};