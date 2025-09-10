#pragma once
#include "Object3D.h"
#include "IObstacle.h"

class Collider;
class Player;
/// <summary>
/// 障害物クラス
/// </summary>
class Obstacle : public IObstacle {
public:// Public variable
	// コンストラクタ
	Obstacle(Object3d* obj, Player* player,Collider* collider);
	// デストラクタ
	~Obstacle() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui() override;

private:// Private method
	/// <summary>
	/// 直る演出
	/// </summary>
	void FixEffect() override;
	/// <summary>
	/// 壊れる演出
	/// </summary>
	void BrokenEffect() override;

private:// Private variable
	uint32_t orizinalTex_;
	uint32_t brokenTex_;
	uint32_t fixedTex_;
};