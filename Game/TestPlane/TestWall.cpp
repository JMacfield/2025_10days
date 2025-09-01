#include "TestWall.h"
#include "../Player/PlayerConfig.h"

using namespace PlayerConfig::FileNames;

TestWall::TestWall(Camera* camera) {
	pCamera_ = camera;

	// ‘Ì‚ÌÀ‘Ì¶¬
	body_ = std::make_unique<Object3d>();
	body_->SetModel("");
}

void TestWall::Init() {

}

void TestWall::Update() {
	// ‘Ì
	body_->Update();
}

void TestWall::Draw() {
	// ‘Ì
	body_->Draw(Textures::body.num, pCamera_);
}

void TestWall::Release() {
	// ‘Ì
	body_.reset();
}
