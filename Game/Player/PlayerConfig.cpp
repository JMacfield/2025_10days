#include "PlayerConfig.h"
#include "Xinput.h"

namespace PlayerConfig {
	namespace Input {
		namespace GamePad {
			bool GamePadTrigger(XINPUT_STATE joyState, XINPUT_STATE preJoyState, int GAMEPAD_NUM) {
				if ((joyState.Gamepad.wButtons & GAMEPAD_NUM) && !(preJoyState.Gamepad.wButtons & GAMEPAD_NUM)) {
					return true;
				}
				else {
					return false;
				}
			}
			SHORT ApplyDeadzone(SHORT inputValue) {
				if (abs(inputValue) < DEADZONE_THRESHOLD / 2) {
					return 0; // デッドゾーン内の入力は無視
				}
				// デッドゾーン外の入力はそのまま返す
				return inputValue;
			}
		}
	}
}