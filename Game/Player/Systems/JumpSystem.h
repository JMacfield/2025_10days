#pragma once
class JumpSystem {
public:
	// �R���X�g���N�^
	JumpSystem() = default;
	// �f�X�g���N�^
	~JumpSystem() = default;

	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

private:
	// �ғ�����
	bool isActive_;
};