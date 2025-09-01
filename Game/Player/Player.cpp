#include "Player.h"
#include "PlayerConfig.h"

using namespace PlayerConfig::FileNames;

Player::Player() {
	// 入力の実体を取得
	input_ = Input::GetInstance();

	// テクスチャ読み込み
	LoadTextures();

	// 体の実体生成
	body_ = std::make_unique<Object3d>();
	body_->SetModel(Models::body.directoryPath + Models::body.name);
}

void Player::Init() {

}

void Player::Update() {
	// 体
	body_->Update();
}

void Player::Draw() {
	// 体
	body_->Draw(Textures::body.num, pCamera_);
}

void Player::Release() {
	// 体
	body_.reset();
}

void Player::LoadTextures() {
	// 体
	Textures::body.num = TextureManager::StoreTexture(Textures::body.name);
}
