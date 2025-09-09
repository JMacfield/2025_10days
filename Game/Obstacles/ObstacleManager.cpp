#include "ObstacleManager.h"
#include "Loader.h"
#include "Collider.h"
#include "../Player/Player.h"
#include "../Player/Components/Math/MathFuncs.h"

using namespace MathFuncs;

ObstacleManager::ObstacleManager(Camera* camera, Player* player) {
	camera_ = camera;
	player_ = player;

	//Loader::LoadJsonFile2("Resources/game/Json", "DemoStage1", objects_, colliders_, wallTypes_);
	Loader::LoadJsonFile2("Resources/game/Json", "TestBlender", objects_, colliders_, wallTypes_);

	for (int i = 0; i < objects_.size(); i++) {
		Obstacle* obstacle = new Obstacle(objects_[i], player_, colliders_[i]);
		obstacle->SetCamera(camera_);
		obstacle->SetDimension(wallTypes_[i]);
		obstacle->Init();
		obstacles_.push_back(obstacle);
	}
}

ObstacleManager::ObstacleManager(std::vector<Object3d*>& objects, std::vector<Collider*>& colliders, std::vector<std::string>& wallType, Camera* camera, Player* player) {
	objects_ = objects;
	colliders_ = colliders;
	wallTypes_ = wallType;
	camera_ = camera;
	player_ = player;

	for (int i = 0; i < objects_.size(); i++) {
		Obstacle* obstacle = new Obstacle(objects_[i], player_, colliders_[i]);
		obstacle->Init();
		obstacle->SetCamera(camera_);
		obstacle->SetDimension(wallTypes_[i]);
		obstacles_.push_back(obstacle);
	}
}

ObstacleManager::~ObstacleManager() {
	for (Collider* c : colliders_) {
		delete c;
	}
	for (Object3d* obj : objects_) {
		delete obj;
	}
	for (Obstacle* obs : obstacles_) {
		delete obs;
	}
}

void ObstacleManager::Init() {
	for (Obstacle* obs : obstacles_) {
		delete obs;
	}
	obstacles_.clear();
	for (int i = 0; i < objects_.size(); i++) {
		Obstacle* obstacle = new Obstacle(objects_[i], player_, colliders_[i]);
		obstacle->SetCamera(camera_);
		obstacle->SetDimension(wallTypes_[i]);
		obstacle->Init();
		obstacles_.push_back(obstacle);
	}
}

void ObstacleManager::Update() {
	for (Obstacle* obs : obstacles_) {
		obs->Update();
	}
}

void ObstacleManager::Draw() {
	for (Obstacle* obs : obstacles_) {
		obs->Draw();
	}
}

void ObstacleManager::Reset() {

}

void ObstacleManager::CheckCollision() {
	for (Collider* collider : colliders_) {
		// 障害物と自機の距離
		float p2o = Length(collider->GetWorldPosition() - GetWorldPosition(player_->GetWorldTransform()->matWorld_));
		if (p2o >= collisionRange) {
			if (collider->GetIsActive()) {
				collider->SetIsActive(false);
			}
		}
		else {
			collider->SetIsActive(true);
		}
	}
}
