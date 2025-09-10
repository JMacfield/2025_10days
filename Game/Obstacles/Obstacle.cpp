#include "Obstacle.h"
#include "../Player/PlayerConfig.h"
#include "../Player/Player.h"
#include "../Player/Components/Math/MathFuncs.h"
#include "Collider.h"
#include "ModelManager.h"
#include "../Utilty/LoaderConfig.h"

using namespace LoaderConfig;
using namespace MathFuncs;
using namespace PlayerConfig::FileNames;

Obstacle::Obstacle(Object3d* obj, Player* player, Collider* collider) {
	// 体の実体生成
	body_ = obj;
	player_ = player;
	collider_ = collider;

	orizinalTex_ = body_->GetTexture();
	brokenTex_ = TextureManager::GetInstance()->StoreTexture("Resources/red.png");
	fixedTex_ = TextureManager::GetInstance()->StoreTexture("Resources/green.png");
}

void Obstacle::Init() {
	currentDimension_ = orizinalDimension_;

	if (ObstacleType::broken == currentDimension_) {
		body_->SetLerpSpeed(1.0f);
		body_->AlphaPingPong10Start(0.01f, 0.6f);
		body_->GlitchVertices(0.3f);
		body_->GlitchVerticesLerp(0.3f);
		body_->ResetVerticesToOriginal();
		body_->SetTexture(brokenTex_);
		collider_->SetIsActive(false);
	}
	if (ObstacleType::fix == currentDimension_) {
		body_->SetLerpSpeed(1.0f);
		body_->AlphaPingPong10Start(0.01f, 0.6f);
		body_->ResetVerticesToOriginal();
		body_->StartLerpToOriginalVertices();
		body_->SetTexture(fixedTex_);
		collider_->SetIsActive(true);
	}
	body_->Update();

	preDimension_ = currentDimension_;
}

void Obstacle::Update() {
	// 過去現在の切り替え
	IObstacle::SwitchDimension();

	// 障害物と自機の距離
	float p2o = Length(GetWorldPosition(player_->GetWorldTransform()->matWorld_) - GetWorldPosition(collider_->worldTransform.matWorld_));
	if (p2o >= collisionRange) {
		collider_->SetIsActive(false);
	}
	else if (ObstacleType::broken != currentDimension_) {
		collider_->SetIsActive(true);
	}

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

void Obstacle::FixEffect() {
	if (ObstacleType::fix != currentDimension_) { return; }
	// 切り替わった瞬間
	if (ObstacleType::fix != preDimension_) {
		// 障害物と自機の距離
		float p2o = Length(GetWorldPosition(player_->GetWorldTransform()->matWorld_) - GetWorldPosition(body_->worldTransform_.matWorld_));
		float lerpSpeed;
		if (p2o <= switchDimensionEffectRange) {
			lerpSpeed = 0.01f;
		}
		else {
			lerpSpeed = 1.0f;
		}
		body_->SetLerpSpeed(lerpSpeed);
		body_->ResetVerticesToOriginal();
		body_->StartLerpToOriginalVertices();
		body_->SetTexture(fixedTex_);
	}

	collider_->SetIsActive(true);
}

void Obstacle::BrokenEffect() {
	if (ObstacleType::broken != currentDimension_) { return; }
	// 切り替わった瞬間
	if (ObstacleType::broken != preDimension_) {
		// 障害物と自機の距離
		float p2o = Length(GetWorldPosition(player_->GetWorldTransform()->matWorld_) - GetWorldPosition(body_->worldTransform_.matWorld_));
		float lerpSpeed;
		if (p2o <= switchDimensionEffectRange) {
			lerpSpeed = 0.01f;
		}
		else {
			lerpSpeed = 1.0f;
		}
		body_->SetLerpSpeed(lerpSpeed);
		body_->GlitchVerticesLerp(0.3f);
		body_->ResetVerticesToOriginal();
		body_->SetTexture(brokenTex_);
	}

	collider_->SetIsActive(false);
}
