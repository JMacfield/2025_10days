#include "TestWall.h"
#include "../Player/PlayerConfig.h"
#include "ModelManager.h"

using namespace PlayerConfig::FileNames;

TestWall::TestWall() {
	// テクスチャ読み込み
	texHandle_ = TextureManager::StoreTexture("Resources/plane/uvChecker.png");

	ModelManager::GetInstance()->LoadModel("Resources/plane/", "plane.gltf");

	// 体の実体生成
	body_ = std::make_unique<Object3d>();
	body_->Init();
	body_->SetModel("plane.gltf");
	body_->worldTransform_.translation_ = { -2.0f,0.0f,4.0f };
	body_->worldTransform_.rotation_.y = (float)std::numbers::pi / 2.0f;
	body_->worldTransform_.scale_ = { 10,10,10 };
}

void TestWall::Init() {

}

void TestWall::Update() {
	// 体
	body_->Update();
}

void TestWall::Draw() {
	// 体
	body_->Draw(texHandle_, pCamera_);
}

void TestWall::Release() {
	// 体
	body_.reset();
}

void TestWall::DebugGui() {
	if (ImGui::TreeNode("TestPlane")) {
		ImGui::DragFloat3("Translation", &body_->worldTransform_.translation_.x, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat3("Rotation", &body_->worldTransform_.rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::TreePop();
	}
}
