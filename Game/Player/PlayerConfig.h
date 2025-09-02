#pragma once
#include <string>
#include "Xinput.h"

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
			inline FileName body = {
				"ball.obj", 
				"Resources/ball/"
			};
		}
		// テクスチャ
		namespace Textures {
			// 体
			inline TexData body = {
				"Resources/ball/uvChecker.png",
				0
			};
		}
	}

	// 入力
	namespace Input {
		// キーボード
		namespace Keyboard {
			// 切り替え
			inline BYTE switching = DIK_RETURN;
			// ジャンプ
			inline BYTE jump = DIK_SPACE;
		}
		// ゲームパッド
		namespace GamePad {
			// 切り替え
			inline BYTE switching;
			// ジャンプ
			inline BYTE jump;
		}
	}
}