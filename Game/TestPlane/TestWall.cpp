#include "TestWall.h"
#include "../Player/PlayerConfig.h"
#include "ModelManager.h"

using namespace PlayerConfig::FileNames;

TestWall::TestWall() {
	// テクスチャ読み込み
	texHandle_ = TextureManager::StoreTexture("Resources/plane/uvChecker.png");

	ModelManager::GetInstance()->LoadModel("Resources/box/", "box.obj");

	// 体の実体生成
	body_ = std::make_unique<Object3d>();
	body_->Init();
	body_->SetModel("box.obj");
	body_->worldTransform_.translation_ = { -2.0f,0.0f,4.0f };
	body_->worldTransform_.scale_ = { 0.1f,8.0f,3.0f };

	//// colliderの設定
	//collider_ = std::make_unique<Collider>();
	//collider_->worldTransform.parent_ = &body_->worldTransform_;
	//collider_->SetOBBLength(body_->worldTransform_.scale_);
	//collider_->SetCollisionPrimitive(kCollisionOBB);
	//collider_->SetCollisionAttribute(kCollisionAttributeObstacles);
	//collider_->SetCollisionMask(~kCollisionAttributeObstacles);
	//collider_->SetOnCollision(std::bind(&TestWall::OnCollision, this, std::placeholders::_1));
	//collider_->SetIsActive(true);
}

void TestWall::Init() {
	body_->SetTexture(texHandle_);
}

void TestWall::Update() {
	// 体
	body_->Update();
}

void TestWall::Draw() {
	// 体
	body_->Draw(pCamera_);
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

void TestWall::OnCollision(Collider* collider) {

}
