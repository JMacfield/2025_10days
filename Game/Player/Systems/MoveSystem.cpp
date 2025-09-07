#include "MoveSystem.h"
#include "imgui.h"
#include "Input.h"
#include "../PlayerConfig.h"
#include "../Components/GameTimer.h"
#include <algorithm>

using namespace PlayerConfig::Input::GamePad;

void MoveSystem::Init() {
	vel_ = { 0.0f, 0.0f, 0.0f };
	rot_ = { 0.0f, 0.0f, 0.0f };
}

void MoveSystem::Update() {
	// 入力処理
	InputUpdate();

	// 速度代入
	vel_ = maxMoveVel * GameTimer::GetInstance()->GetTimeScale();
}

void MoveSystem::DebugGui() {
	ImGui::DragFloat3("MaxMoveVel", &maxMoveVel.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("CurrentVel", &vel_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat2("Sensitivity", &sensitivity.x, 0.001f, -100.0f, 100.0f);
}

void MoveSystem::InputUpdate() {
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		rot_.y -= 0.05f;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		rot_.y += 0.05f;
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		rot_.x -= 0.05f;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		rot_.x += 0.05f;
	}

	// ゲームパッド
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		rot_.y += (float)ApplyDeadzone(joyState.Gamepad.sThumbRX) / SHRT_MAX * sensitivity.y;
		rot_.x -= (float)ApplyDeadzone(joyState.Gamepad.sThumbRY) / SHRT_MAX * sensitivity.x;
	}

	rot_.x = std::clamp<float>(rot_.x, -1.5f, 1.5f);
}
