#pragma once
#include "Vector3.h"
#include <string>

/// <summary>
/// ローダーの設定
/// </summary>
namespace LoaderConfig {
	// 当たり判定
	namespace Collision {
		namespace Mask {
			// 壁
			inline std::string wall = "WALL";
			// 攻撃
			inline std::string attack = "ATTACK";
			// ビーム
			inline std::string beam = "BEAM";
			// クリア
			inline std::string clear = "CLEAR";
			// チェックポイント
			inline std::string checkPoint = "CHECKPOINT";
		}
	}
	// オブジェクトのタイプ
	namespace ObstacleType {
		// 壊れている
		inline std::string broken = "BROKEN";
		// 直った
		inline std::string fix = "FIXED";
		// 変わらない
		inline std::string abs = "ABS";
	}

	namespace FilePath {
		namespace Texture {
			inline std::string filePath = "Resources/game/";
		}
		namespace Model {
			inline std::string filePath = "Resources/game/";
		}
	}
}