#include "IObstacle.h"
#include "../Player/Player.h"
#include "../Utilty/LoaderConfig.h"

using namespace LoaderConfig;

void IObstacle::SwitchDimension() {
	if (ObstacleType::abs == currentDimension_) { return; }

	// 障害物の状態を切り替える
	if (player_->GetTriggerSwitchDimension()) {
		// 壊す
		if (ObstacleType::fix == currentDimension_) {
			currentDimension_ = ObstacleType::broken;
			// 壊れる演出
			BrokenEffect();
		}
		// 直る
		else if (ObstacleType::broken == currentDimension_) {
			currentDimension_ = ObstacleType::fix;
			// 直る演出
			FixEffect();
		}
	}
}