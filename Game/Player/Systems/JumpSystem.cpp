#include "JumpSystem.h"
#include "../PlayerConfig.h"
#include "../Player.h"
#include "../Components/Math/MathFuncs.h"

using namespace MathFuncs;
using namespace PlayerConfig::Input;

JumpSystem::JumpSystem(Player* player) {
	input_ = Input::GetInstance();
	player_ = player;
}

JumpSystem::~JumpSystem() {

}

void JumpSystem::Init() {
	vel_ = { 0.0f,0.0f,0.0f };

	airJumpCount_ = 0;

	isActive_ = false;
}

void JumpSystem::Update() {
	// 入力処理
	InputUpdate();

	// ジャンプ時の速度減衰
	if (isActive_) {
		vel_.x = Lerps::ExponentialInterpolate(vel_.x, jumpDirX_ * firstVel.x / 10.0f * 2.0f, 0.1f);
		vel_.z = Lerps::ExponentialInterpolate(vel_.z, firstVel.z / 10.0f * 2.0f, 0.1f);
	}

	// 着地したら初期化
	if (player_->GetIsLanding()) {
		airJumpCount_ = 0;
		player_->EndJump();
		vel_ = { 0.0f,0.0f,0.0f };
	}

	preJoyState = joyState;
}

void JumpSystem::DebugGui() {
	ImGui::DragFloat3("FirstVel", &firstVel.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("CurrentVel", &vel_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragInt("AirJumpCount", &airJumpCount_);
	ImGui::DragFloat("JumpDirX", &jumpDirX_);
	ImGui::Checkbox("IsActive", &isActive_);
}

void JumpSystem::InputUpdate() {
	// 空中ジャンプのできる回数を超えている場合はジャンプさせない
	if (airJumpCount_ >= maxAirJump) { return; }

	// ジャンプ開始
	if (input_->TriggerKey(Keyboard::jump)) {
		if (player_->GetIsAir()) {
			airJumpCount_++;
		}
		isActive_ = true;
		player_->StartJump();
		// どちらの方向にジャンプするかを決める
		JumpSideUpdate();
	}
	else if (input_->GetJoystickState(joyState)) {
		if (PlayerConfig::Input::GamePad::GamePadTrigger(joyState, preJoyState, XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			if (player_->GetIsAir()) {
				airJumpCount_++;
			}
			isActive_ = true;
			player_->StartJump();
			// どちらの方向にジャンプするかを決める
			JumpSideUpdate();
		}
	}
}

void JumpSystem::JumpSideUpdate() {
	// 右に向かって飛ぶ
	if (player_->GetCurrentWallSide() == WallSide::kLeft) {
		jumpDirX_ = 1.0f;
		Vector3 vel = firstVel;
		vel_ += vel;
	}
	// 左に向かって飛ぶ
	else if (player_->GetCurrentWallSide() == WallSide::kRight) {
		jumpDirX_ = -1.0f;
		Vector3 vel = firstVel;
		vel.x *= jumpDirX_;
		vel_ += vel;
	}
}
