#pragma once
class MoveSystem {
public:
	// �R���X�g���N�^
	MoveSystem() = default;
	// �f�X�g���N�^
	~MoveSystem() = default;

	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

private:// Private method
	/// <summary>
	/// ���͏���
	/// </summary>
	void InputUpdate();

private:
	// �ғ�����
	bool isActive_;
};