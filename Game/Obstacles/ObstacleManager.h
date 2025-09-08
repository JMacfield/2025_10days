#pragma once
#include "Obstacle.h"
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

public:// Accessor method
#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

private:// 外部から受け取る変数
	std::vector<Object3d*> objects_;
	std::vector<Collider*> colliders_;
	std::vector<std::string> wallTypes_;
	Camera* camera_;
	Player* player_;

private:
	std::vector<Obstacle*> obstacles_;
};