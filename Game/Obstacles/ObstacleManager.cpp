#include "ObstacleManager.h"
#include "Loader.h"
#include "Collider.h"
#include "../Player/Player.h"
#include "../Player/Components/Math/MathFuncs.h"
#include "Obstacle.h"
#include "Beam.h"

using namespace MathFuncs;

ObstacleManager::ObstacleManager(Camera* camera, Player* player) {
	camera_ = camera;
	player_ = player;

	//Loader::LoadJsonFile2("Resources/game/Json", "DemoStage1", objects_, colliders_, wallTypes_);
	Loader::LoadJsonFile2("Resources/game/Json", "TestBlender", objects_, colliders_, wallTypes_);

	for (int i = 0; i < objects_.size(); i++) {
		if (colliders_[i]->GetCollisionAttribute() == kCollisionAttributeBeam) {
			Beam* beam = new Beam(objects_[i], player_, colliders_[i]);
			beam->SetCamera(camera_);
			beam->SetDimension(wallTypes_[i]);
			beam->Init();
			obstacles_.push_back(beam);
		}
		else {
			Obstacle* obstacle = new Obstacle(objects_[i], player_, colliders_[i]);
			obstacle->SetCamera(camera_);
			obstacle->SetDimension(wallTypes_[i]);
			obstacle->Init();
			obstacles_.push_back(obstacle);
		}
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
	for (IObstacle* obs : obstacles_) {
		delete obs;
	}
}

void ObstacleManager::Init() {

}

void ObstacleManager::Update() {
	for (IObstacle* obs : obstacles_) {
		if (obs->GetCollider()->GetCollisionAttribute() == kCollisionAttributeClear) { continue; }
		obs->Update();
	}
}

void ObstacleManager::Draw() {
	for (IObstacle* obs : obstacles_) {
		if (obs->GetCollider()->GetCollisionAttribute() == kCollisionAttributeClear) { continue; }
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
