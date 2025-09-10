#include "Beam.h"
#include "../Player/PlayerConfig.h"
#include "../Player/Player.h"
#include "../Player/Components/Math/MathFuncs.h"
#include "Collider.h"
#include "ModelManager.h"
#include "../Utilty/LoaderConfig.h"

using namespace LoaderConfig;
using namespace MathFuncs;
using namespace PlayerConfig::FileNames;

Beam::Beam(Object3d* obj, Player* player, Collider* collider) {
	// 体の実体生成
	body_ = obj;
	player_ = player;
	collider_ = collider;

	orizinalScale_ = body_->worldTransform_.scale_;

	orizinalTex_ = body_->GetTexture();
	fixedTex_ = TextureManager::GetInstance()->StoreTexture("Resources/red.png");
}

void Beam::Init() {
	currentDimension_ = orizinalDimension_;

	if (ObstacleType::broken == currentDimension_) {
		body_->worldTransform_.scale_.x = 0.1f;
		body_->worldTransform_.scale_.y = 0.1f;
		body_->AlphaPingPong10Start(0.01f, 0.6f);
		body_->SetTexture(orizinalTex_);
		collider_->SetIsActive(false);
	}
	if (ObstacleType::fix == currentDimension_) {
		body_->worldTransform_.scale_ = orizinalScale_;
		body_->AlphaPingPong10Start(0.6f, 1.0f);
		body_->SetTexture(fixedTex_);
		collider_->SetIsActive(true);
	}

	preDimension_ = currentDimension_;
}

void Beam::Update() {
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

void Beam::Draw() {
	// 壊れてないときのみ描画
	//if (ObstacleType::broken != currentDimension_) {
		body_->Draw(pCamera_);
	//}
}

void Beam::DebugGui() {
	if (ImGui::TreeNode("Beam")) {
		ImGui::DragFloat3("Translation", &body_->worldTransform_.translation_.x, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat3("Rotation", &body_->worldTransform_.rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::TreePop();
	}
}

void Beam::FixEffect() {
	if (ObstacleType::fix != currentDimension_) { return; }
	// 切り替わった瞬間
	if (ObstacleType::fix != preDimension_) {

	}
	body_->worldTransform_.scale_ = orizinalScale_;
	body_->SetTexture(fixedTex_);
	body_->AlphaPingPong10Start(0.01f, 0.6f);

	collider_->SetIsActive(true);
}

void Beam::BrokenEffect() {
	if (ObstacleType::broken != currentDimension_) { return; }
	// 切り替わった瞬間
	if (ObstacleType::broken != preDimension_) {
		
	}
	body_->worldTransform_.scale_.x = 0.1f;
	body_->worldTransform_.scale_.y = 0.1f;
	body_->SetTexture(orizinalTex_);
	body_->AlphaPingPong10Start(0.6f, 1.0f);

	collider_->SetIsActive(false);
}

