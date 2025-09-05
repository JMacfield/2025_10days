#pragma once
#include "Vector3.h"
#include <string>

/// <summary>
/// ローダーの設定
/// </summary>
namespace LoaderConfig {
	// 当たり判定
	namespace Collider {
		namespace Mask {
			// 壁
			inline std::string wall = "BOX";

			// 攻撃
			inline std::string attack = "ATTACK";
		}
	}
}