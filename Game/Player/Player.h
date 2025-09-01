#pragma once
#include "Input.h"
#include "Object3D.h"

class Player {
public:// Public variable
	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	~Player() = default;

	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();
	/// <summary>
	/// �������
	/// </summary>
	void Release();

private:// Private method 
	/// <summary>
	/// �e�N�X�`���̓ǂݍ���
	/// </summary>
	void LoadTextures();

public:// Accessor method
#pragma region Getter
	/// <summary>
	/// �̂�WorldTF�̃A�h���X���擾
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &body_->worldTransform_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// �J�����̃A�h���X��ݒ�
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { pCamera_ = camera; }
#pragma endregion

private:// Engine system
	// ����
	Input* input_;

private:// �O������󂯎��A�h���X
	// �J����
	Camera* pCamera_;

private:// Private variable
	// �̂̃I�u�W�F�N�g
	std::unique_ptr<Object3d> body_;
};

