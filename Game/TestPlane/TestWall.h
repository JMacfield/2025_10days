#pragma once
#include "Object3D.h"

class TestWall {
public:// Public variable
	// �R���X�g���N�^
	TestWall(Camera* camera);
	// �f�X�g���N�^
	~TestWall() = default;

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


private:// �O������󂯎��A�h���X
	// �J����
	Camera* pCamera_;

private:// Private variable
	// �̂̃I�u�W�F�N�g
	std::unique_ptr<Object3d> body_;
};

