#pragma once
#include "Input.h"
#include "Object3D.h"

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

private:// Private method 
	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	void LoadTextures();

public:// Accessor method
#pragma region Getter
	/// <summary>
	/// 体のWorldTFのアドレスを取得
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &body_->worldTransform_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// カメラのアドレスを設定
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { pCamera_ = camera; }
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
};

