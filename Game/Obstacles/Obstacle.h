#pragma once
#include "Object3D.h"

class Collider;
class Player;
/// <summary>
/// 障害物クラス
/// </summary>
class Obstacle {
public:// Public variable
	// コンストラクタ
	Obstacle(Object3d* obj, Player* player,Collider* collider);
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

private:// 調整項目
	// 次元を切り替えた時の演出を出す範囲
	float switchDimensionEffectRange = 50.0f;

	// 当たり判定をとる距離
	float collisionRange = 10.0f;

private:// 外部から受け取るアドレス
	// カメラ
	Camera* pCamera_;
	// 自機
	Player* player_;
	// 当たり判定
	Collider* collider_;

private:// Private variable
	int time = 0;
	// 体のオブジェクト
	Object3d* body_;

	// 障害物の状態
	std::string currentDimension_;
	std::string preDimension_;
};