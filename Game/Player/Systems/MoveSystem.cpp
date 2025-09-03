#include "MoveSystem.h"
#include "imgui.h"
#include "Input.h"
#include <algorithm>

void MoveSystem::Init() {
	vel_ = { 0.0f, 0.0f, 0.0f };
}

void MoveSystem::Update() {
	// 入力処理
	InputUpdate();

	// 速度代入
	vel_ = maxMoveVel;
}

void MoveSystem::DebugGui() {
	ImGui::DragFloat3("MaxMoveVel", &maxMoveVel.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("CurrentVel", &vel_.x, 0.1f, -100.0f, 100.0f);
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

	rot_.x = std::clamp<float>(rot_.x, -2.0f, 2.0f);
}
