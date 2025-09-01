#pragma once
#include <string>

struct FileName {
	std::string name;			// ファイル名
	std::string directoryPath;	// ファイル名までのパス
};
struct TexData {
	std::string name;	// Resourcesからファイル名まで
	uint32_t num;		// 識別番号
};

/// <summary>
/// 自機の設定
/// </summary>
namespace PlayerConfig {
	// ファイル名
	namespace FileNames {
		// モデル
		namespace Models {
			// 体
			inline FileName body;
		}
		// テクスチャ
		namespace Textures {
			inline TexData body;
		}
	}
}