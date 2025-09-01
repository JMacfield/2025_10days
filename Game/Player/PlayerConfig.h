#pragma once
#include <string>

struct FileName {
	std::string name;			// �t�@�C����
	std::string directoryPath;	// �t�@�C�����܂ł̃p�X
};
struct TexData {
	std::string name;	// Resources����t�@�C�����܂�
	uint32_t num;		// ���ʔԍ�
};

/// <summary>
/// ���@�̐ݒ�
/// </summary>
namespace PlayerConfig {
	// �t�@�C����
	namespace FileNames {
		// ���f��
		namespace Models {
			// ��
			inline FileName body;
		}
		// �e�N�X�`��
		namespace Textures {
			inline TexData body;
		}
	}
}