#pragma once
#include <string>
#include "Input.h"

struct FileName {
	std::string name;			// ファイル名
	std::string directoryPath;	// ファイル名までのパス
};
struct TexData {
	std::string name;	// Resourcesからファイル名まで
	uint32_t num;		// 識別番号
};
// 自機が切り替えられる次元の種類
enum class DimensionType {
	kNow,	// 現在
	kPast	// 過去
};
// 自機がどちらの壁にいるか
enum class WallSide {
	kNone,
	kRight,
	kLeft,
	kCount
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

			// デッドゾーンの閾値
			inline int DEADZONE_THRESHOLD = 8000;

			/// <summary>
			/// ゲームパッドのボタンを押した瞬間を取得
			/// </summary>
			/// <param name="joyState"></param>
			/// <param name="preJoyState"></param>
			/// <param name="GAMEPAD_NUM"></param>
			/// <returns></returns>
			bool GamePadTrigger(XINPUT_STATE joyState, XINPUT_STATE preJoyState, int GAMEPAD_NUM);
			/// <summary>
			/// デッドゾーンを適用する関数
			/// </summary>
			/// <param name="inputValue"></param>
			/// <returns></returns>
			SHORT ApplyDeadzone(SHORT inputValue);
		}

	}
}