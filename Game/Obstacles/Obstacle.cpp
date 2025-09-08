#include "Obstacle.h"
#include "../Player/PlayerConfig.h"
#include "../Player/Player.h"
#include "Collider.h"
#include "ModelManager.h"
#include "../Utilty/LoaderConfig.h"

using namespace LoaderConfig;
using namespace PlayerConfig::FileNames;

Obstacle::Obstacle(Object3d* obj, Player* player, Collider* collider) {
	// 体の実体生成
	body_ = obj;
	player_ = player;
	collider_ = collider;
}

void Obstacle::Init() {
	if (ObstacleType::broken == currentDimension_) {
		body_->AlphaPingPong10Start(0.01f, 0.6f);
		body_->GlitchVertices(0.3f);
	}
	if (ObstacleType::fix == currentDimension_) {
		body_->AlphaPingPong10Start(0.01f, 0.6f);
	}
}

void Obstacle::Update() {
	time++;
	// 過去現在の切り替え
	SwitchDimension();

	// 体
	body_->Update();

	preDimension_ = currentDimension_;
}

void Obstacle::Draw() {
	// 体
	// 直る
	if (ObstacleType::fix == currentDimension_) {
		body_->Draw(pCamera_);
	}
	// 壊す
	else if (ObstacleType::broken == currentDimension_) {
		body_->Draw(pCamera_);
	}
	else {
		body_->Draw(pCamera_);
	}
}

void Obstacle::DebugGui() {
	if (ImGui::TreeNode("TestPlane")) {
		ImGui::DragFloat3("Translation", &body_->worldTransform_.translation_.x, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat3("Rotation", &body_->worldTransform_.rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::TreePop();
	}
}

void Obstacle::SwitchDimension() {
	if (ObstacleType::abs == currentDimension_) { return; }

	// 障害物の状態を切り替える
	if (player_->GetTriggerSwitchDimension()) {
		// 壊す
		if (ObstacleType::fix == currentDimension_) {
			currentDimension_ = ObstacleType::broken;
			// 壊れる演出
			BrokenEffect();
			collider_->SetIsActive(false);
		}
		// 直る
		else if (ObstacleType::broken == currentDimension_) {
			currentDimension_ = ObstacleType::fix;
			// 直る演出
			FixEffect();
			collider_->SetIsActive(true);
		}
	}


}

void Obstacle::FixEffect() {
	if (ObstacleType::fix != currentDimension_) { return; }
	// 切り替わった瞬間
	if (ObstacleType::fix != preDimension_) {
		body_->AlphaPingPong10Start(0.01f, 0.6f);
		body_->SetLerpSpeed(0.01f);
		body_->GlitchVerticesLerp(0.3f);
		body_->ResetVerticesToOriginal();
		body_->StartLerpToOriginalVertices();
	}
}

void Obstacle::BrokenEffect() {
	if (ObstacleType::broken != currentDimension_) { return; }
	// 切り替わった瞬間
	if (ObstacleType::broken != preDimension_) {
		body_->AlphaPingPong10Start(0.01f, 0.6f);
		body_->SetLerpSpeed(0.01f);
		body_->GlitchVerticesLerp(0.3f);
		body_->ResetVerticesToOriginal();
	}
}
