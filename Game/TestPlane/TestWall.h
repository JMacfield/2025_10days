#pragma once
#include "Object3D.h"

class TestWall {
public:// Public variable
	// コンストラクタ
	TestWall(Camera* camera);
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

private:// Private method


private:// 外部から受け取るアドレス
	// カメラ
	Camera* pCamera_;

private:// Private variable
	// 体のオブジェクト
	std::unique_ptr<Object3d> body_;
};

