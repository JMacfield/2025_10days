#pragma once
#include "Camera.h"
#include "Input.h"
#include "WorldTransform.h"

/// <summary>
/// �Ǐ]�J����
/// </summary>
class FollowCamera {
public:// Public variable
	// �R���X�g���N�^
	FollowCamera();
	// �f�X�g���N�^
	~FollowCamera() = default;

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
	/// ���͏���
	/// </summary>
	void InputUpdate();

private:// Engine system
	// ����
	Input* input_;

private:// Private variable
	// �J����
	Camera* pCamera_;
};