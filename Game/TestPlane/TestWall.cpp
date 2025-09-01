#include "TestWall.h"
#include "../Player/PlayerConfig.h"

using namespace PlayerConfig::FileNames;

TestWall::TestWall(Camera* camera) {
	pCamera_ = camera;

	// �̂̎��̐���
	body_ = std::make_unique<Object3d>();
	body_->SetModel("");
}

void TestWall::Init() {

}

void TestWall::Update() {
	// ��
	body_->Update();
}

void TestWall::Draw() {
	// ��
	body_->Draw(Textures::body.num, pCamera_);
}

void TestWall::Release() {
	// ��
	body_.reset();
}
