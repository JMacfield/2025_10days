#include "MoveSystem.h"
#include "imgui.h"

void MoveSystem::Init() {
	vel_ = { 0.0f, 0.0f, 0.0f };
}

void MoveSystem::Update() {
	// 速度代入
	vel_ = maxMoveVel;
}

void MoveSystem::DebugGui() {
	ImGui::DragFloat3("MaxMoveVel", &maxMoveVel.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("CurrentVel", &vel_.x, 0.1f, -100.0f, 100.0f);
}

void MoveSystem::InputUpdate() {

}
