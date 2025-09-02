#include "Player.h"
#include "PlayerConfig.h"
#include "ModelManager.h"

using namespace PlayerConfig::FileNames;

Player::Player() {
	// 入力の実体を取得
	input_ = Input::GetInstance();

	// テクスチャ読み込み
	LoadTextures();

	ModelManager::GetInstance()->LoadModel(Models::body.directoryPath, Models::body.name);

	// 体の実体生成
	body_ = std::make_unique<Object3d>();
	body_->Init();
	body_->SetModel(Models::body.name);

	// 全ての機能生成
	CreateSystems();

	currentWallSide_ = WallSide::kLeft;
}

void Player::Init() {
	body_->worldTransform_.translation_ = { 0,0,0 };
	body_->worldTransform_.rotation_ = { 0,0,0 };
	body_->worldTransform_.scale_ = { 1,1,1 };

	moveSystem_->Init();
	jumpSystem_->Init();

	// 空中にいるか
	isAir_ = false;
	// 着地したか
	isLanding_ = true;
	currentWallSide_ = WallSide::kLeft;
}

void Player::Update() {
	// 移動処理
	moveSystem_->Update();
	// ジャンプ
	jumpSystem_->Update();

	// 速度代入
	body_->worldTransform_.translation_ += moveSystem_->GetVel() + jumpSystem_->GetVel();

	// 体
	body_->Update();

	// 着地している
	if (body_->worldTransform_.translation_.y <= -0.1f) {
		EndJump();
	}
}

void Player::Draw() {
	// 体
	body_->Draw(Textures::body.num, pCamera_);
}

void Player::Release() {
	// 体
	body_.reset();
	// 移動
	moveSystem_.reset();
	// ジャンプ
	jumpSystem_.reset();
}

void Player::DebugGui() {
	if (ImGui::TreeNode("Player")) {
		if (ImGui::TreeNode("Object")) {
			ImGui::DragFloat3("Translation", &body_->worldTransform_.translation_.x, 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat3("Rotation", &body_->worldTransform_.rotation_.x, 0.01f, -6.28f, 6.28f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Jump")) {
			jumpSystem_->DebugGui();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Move")) {
			moveSystem_->DebugGui();
			ImGui::TreePop();
		}
		if (ImGui::Button("Reset")) {
			Init();
		}
		ImGui::TreePop();
	}
}

void Player::LoadTextures() {
	// 体
	Textures::body.num = TextureManager::StoreTexture(Textures::body.name);
}

void Player::CreateSystems() {
	// 移動処理
	moveSystem_ = std::make_unique<MoveSystem>();
	moveSystem_->Init();

	// ジャンプ処理
	jumpSystem_ = std::make_unique<JumpSystem>(this);
	jumpSystem_->Init();
}
