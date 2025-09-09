#pragma once
#include "IObstacle.h"
#include "../Camera/FollowCamera.h"

class Collider;
class Player;
class ObstacleManager {
public:
	// コンストラクタ
	ObstacleManager(Camera* camera, Player* player);
	ObstacleManager(std::vector<Object3d*>& objects, std::vector<Collider*>& colliders, std::vector<std::string>& wallType, Camera* camera, Player* player);
	// デストラクタ
	~ObstacleManager();

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
	/// リセット
	/// </summary>
	void Reset();

private:// Private method
	/// <summary>
	/// 自機と一定距離にいないときは当たり判定を消す
	/// </summary>
	void CheckCollision();

public:// Accessor method
#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

private:// 調整項目
	// 当たり判定をとる距離
	float collisionRange = 10.0f;

private:// 外部から受け取る変数
	std::vector<Object3d*> objects_;
	std::vector<Collider*> colliders_;
	std::vector<std::string> wallTypes_;
	Camera* camera_;
	Player* player_;

private:
	std::vector<IObstacle*> obstacles_;
};