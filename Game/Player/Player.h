#pragma once
#include "Input.h"
#include "Object3D.h"
#include "Collider.h"
#include "Systems/MoveSystem.h"
#include "Systems/JumpSystem.h"

class Player {
public:
	// どちらの壁にいるか
	enum class WallSide {
		kRight,
		kLeft,
		kCount
	};

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

public:// Accessor method
	/// <summary>
	/// ジャンプ開始
	/// </summary>
	void StartJump() {
		// 空中にいるか
		isAir_ = true;
		// 着地したか
		isLanding_ = false;
	}
	/// <summary>
	/// ジャンプ終了
	/// </summary>
	void EndJump() {
		if (!isAir_ && isLanding_) { return; }

		// 右に向かって飛ぶ
		if (currentWallSide_ == WallSide::kLeft) {
			currentWallSide_ = WallSide::kRight;
		}
		// 左に向かって飛ぶ
		else if (currentWallSide_ == WallSide::kRight) {
			currentWallSide_ = WallSide::kLeft;
		}
		// 空中にいるか
		isAir_ = false;
		// 着地したか
		isLanding_ = true;
	}

#pragma region Getter
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
	/// 空中にいるかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAir() { return isAir_; }
	/// <summary>
	/// 着地しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsLanding() { return isLanding_; }
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

private:// Engine system
	// 入力
	Input* input_;

private:// 外部から受け取るアドレス
	// カメラ
	Camera* pCamera_;

private:// Private variable
	// 体のオブジェクト
	std::unique_ptr<Object3d> body_;

	Collider* collider_;

	// 移動処理
	std::unique_ptr<MoveSystem> moveSystem_;
	// ジャンプ処理
	std::unique_ptr<JumpSystem> jumpSystem_;

	// 現在壁の右側化左側にいるか
	WallSide currentWallSide_;

	// 空中にいるか
	bool isAir_ = false;
	// 着地したか
	bool isLanding_ = true;
};

