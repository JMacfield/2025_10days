#include "ObstacleManager.h"
#include "Loader.h"
#include "Collider.h"

ObstacleManager::ObstacleManager(Camera* camera, Player* player) {
	camera_ = camera;
	player_ = player;

	Loader::LoadJsonFile2("Resources/game/Json", "DemoStage1", objects_, colliders_, wallTypes_);

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
