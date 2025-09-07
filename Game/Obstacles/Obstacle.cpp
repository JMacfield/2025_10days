#include "Obstacle.h"
#include "../Player/PlayerConfig.h"
#include "ModelManager.h"
#include "../Utilty/LoaderConfig.h"

using namespace LoaderConfig;
using namespace PlayerConfig::FileNames;

Obstacle::Obstacle(Object3d* obj) {
	// �̂̎��̐���
	body_ = obj;
}

void Obstacle::Init() {

}

void Obstacle::Update() {
	// �ߋ����݂̐؂�ւ�
	SwitchDimension();

	// ��
	body_->Update();

	preDimension_ = currentDimension_;
}

void Obstacle::Draw() {
	// ��
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

	// ��
	if (ObstacleType::fix == currentDimension_) { 
		currentDimension_ = ObstacleType::broken;
	}
	// ����
	else if(ObstacleType::broken == currentDimension_) {
		currentDimension_ = ObstacleType::fix;
	}

	// ���鉉�o
	FixEffect();
	// ���鉉�o
	BrokenEffect();
}

void Obstacle::FixEffect() {
	if (ObstacleType::fix != currentDimension_) { return; }

	// �؂�ւ�����u��
	if (ObstacleType::fix != preDimension_) {

	}
}

void Obstacle::BrokenEffect() {
	if (ObstacleType::broken != currentDimension_) { return; }

	// �؂�ւ�����u��
	if (ObstacleType::broken != preDimension_) {

	}
}
