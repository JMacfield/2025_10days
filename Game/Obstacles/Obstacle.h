#pragma once
#include "Object3D.h"
#include "Collider.h"

class Obstacle {
public:// Public variable
	// コンストラクタ
	Obstacle(Object3d* obj);
	// デストラクタ
	~Obstacle() = default;

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
	/// 調整項目
	/// </summary>
	void DebugGui();

private:// Private method
	/// <summary>
	/// 過去現在の切り替え
	/// </summary>
	void SwitchDimension();

	/// <summary>
	/// 直る演出
	/// </summary>
	void FixEffect();
	/// <summary>
	/// 壊れる演出
	/// </summary>
	void BrokenEffect();

public:// Accessor method
	/// <summary>
	/// カメラのアドレスを設定
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { pCamera_ = camera; }
	/// <summary>
	/// テクスチャの設定
	/// </summary>
	/// <param name="texHandle"></param>
	//void SetTexture(const uint32_t& texHandle){}
	/// <summary>
	/// 次元の設定
	/// </summary>
	/// <param name="dimension"></param>
	void SetDimension(const std::string& dimension) { currentDimension_ = dimension; }

private:// 外部から受け取るアドレス
	// カメラ
	Camera* pCamera_;

private:// Private variable
	// 体のオブジェクト
	Object3d* body_;

	// 障害物の状態
	std::string currentDimension_;
	std::string preDimension_;
};