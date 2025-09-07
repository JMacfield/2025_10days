#pragma once
#include "Object3D.h"
#include "Collider.h"

class Obstacle {
public:// Public variable
	// �R���X�g���N�^
	Obstacle(Object3d* obj);
	// �f�X�g���N�^
	~Obstacle() = default;

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
	/// ��������
	/// </summary>
	void DebugGui();

private:// Private method
	/// <summary>
	/// �ߋ����݂̐؂�ւ�
	/// </summary>
	void SwitchDimension();

	/// <summary>
	/// ���鉉�o
	/// </summary>
	void FixEffect();
	/// <summary>
	/// ���鉉�o
	/// </summary>
	void BrokenEffect();

public:// Accessor method
	/// <summary>
	/// �J�����̃A�h���X��ݒ�
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { pCamera_ = camera; }
	/// <summary>
	/// �e�N�X�`���̐ݒ�
	/// </summary>
	/// <param name="texHandle"></param>
	//void SetTexture(const uint32_t& texHandle){}
	/// <summary>
	/// �����̐ݒ�
	/// </summary>
	/// <param name="dimension"></param>
	void SetDimension(const std::string& dimension) { currentDimension_ = dimension; }

private:// �O������󂯎��A�h���X
	// �J����
	Camera* pCamera_;

private:// Private variable
	// �̂̃I�u�W�F�N�g
	Object3d* body_;

	// ��Q���̏��
	std::string currentDimension_;
	std::string preDimension_;
};