#include "Player.h"
#include "PlayerConfig.h"

using namespace PlayerConfig::FileNames;

Player::Player() {
	// ���͂̎��̂��擾
	input_ = Input::GetInstance();

	// �e�N�X�`���ǂݍ���
	LoadTextures();

	// �̂̎��̐���
	body_ = std::make_unique<Object3d>();
	body_->SetModel(Models::body.directoryPath + Models::body.name);
}

void Player::Init() {

}

void Player::Update() {
	// ��
	body_->Update();
}

void Player::Draw() {
	// ��
	body_->Draw(Textures::body.num, pCamera_);
}

void Player::Release() {
	// ��
	body_.reset();
}

void Player::LoadTextures() {
	// ��
	Textures::body.num = TextureManager::StoreTexture(Textures::body.name);
}
