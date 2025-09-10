#pragma once
#include "Object3D.h"

class Collider;
class Player;
/// <summary>
/// 障害物クラス
/// </summary>
class IObstacle {
public:// Public variable
	// コンストラクタ
	IObstacle() = default;
	// デストラクタ
	virtual ~IObstacle() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 調整項目
	/// </summary>
	virtual void DebugGui() = 0;

protected:// Private method
	/// <summary>
	/// 過去現在の切り替え
	/// </summary>
	void SwitchDimension();

	/// <summary>
	/// 直る演出
	/// </summary>
	virtual void FixEffect() = 0;
	/// <summary>
	/// 壊れる演出
	/// </summary>
	virtual void BrokenEffect() = 0;

public:// Accessor method
	Collider* GetCollider() { return collider_; }

	/// <summary>
	/// カメラのアドレスを設定
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { pCamera_ = camera; }
	/// <summary>
	/// 次元の設定
	/// </summary>
	/// <param name="dimension"></param>
	void SetDimension(const std::string& dimension) { 
		currentDimension_ = dimension;
		orizinalDimension_ = dimension;
	}

protected:// 調整項目
	// 次元を切り替えた時の演出を出す範囲
	float switchDimensionEffectRange = 50.0f;

	// 当たり判定をとる距離
	float collisionRange = 20.0f;

protected:// 外部から受け取るアドレス
	// カメラ
	Camera* pCamera_;
	// 自機
	Player* player_;
	// 当たり判定
	Collider* collider_;

protected:// Private variable
	// 体のオブジェクト
	Object3d* body_;

	// 障害物の状態
	std::string currentDimension_;
	std::string preDimension_;
	std::string orizinalDimension_;
};