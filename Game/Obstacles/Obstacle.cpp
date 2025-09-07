#include "Obstacle.h"
#include "../Player/PlayerConfig.h"
#include "ModelManager.h"
#include "../Utilty/LoaderConfig.h"

using namespace LoaderConfig;
using namespace PlayerConfig::FileNames;

Obstacle::Obstacle(Object3d* obj) {
	// ëÃÇÃé¿ëÃê∂ê¨
	body_ = obj;
}

void Obstacle::Init() {

}

void Obstacle::Update() {
	// âﬂãéåªç›ÇÃêÿÇËë÷Ç¶
	SwitchDimension();

	// ëÃ
	body_->Update();

	preDimension_ = currentDimension_;
}

void Obstacle::Draw() {
	// ëÃ
	//body_->Draw(pCamera_);
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

	// âÛÇ∑
	if (ObstacleType::fix == currentDimension_) { 
		currentDimension_ = ObstacleType::broken;
	}
	// íºÇÈ
	else if(ObstacleType::broken == currentDimension_) {
		currentDimension_ = ObstacleType::fix;
	}

	// íºÇÈââèo
	FixEffect();
	// âÛÇÍÇÈââèo
	BrokenEffect();
}

void Obstacle::FixEffect() {
	if (ObstacleType::fix != currentDimension_) { return; }

	// êÿÇËë÷ÇÌÇ¡ÇΩèuä‘
	if (ObstacleType::fix != preDimension_) {

	}
}

void Obstacle::BrokenEffect() {
	if (ObstacleType::broken != currentDimension_) { return; }

	// êÿÇËë÷ÇÌÇ¡ÇΩèuä‘
	if (ObstacleType::broken != preDimension_) {

	}
}
