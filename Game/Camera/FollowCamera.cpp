#include "FollowCamera.h"
#include "../Player/Components/Math/MathFuncs.h"
#include "../Player/Player.h"

using namespace MathFuncs;

FollowCamera::FollowCamera(Player* player) {
	player_ = player;

	input_ = Input::GetInstance();

	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	currentFov_ = defaultFov;

	fovAnim_[FovType::kDead].SetAnimData(&currentFov_, 0.0f, deadShakeFrame, 12.0f, Easings::EaseOutExpo);
	fovAnim_[FovType::kAir].SetAnimData(&currentFov_, 0.0f, airFov, 12.0f, Easings::EaseOutExpo);
	fovAnim_[FovType::kLanding].SetAnimData(&currentFov_, 0.0f, landingFov, 5.0f, Easings::EaseOutExpo);
	fovAnim_[FovType::kLanding].SetAnimData(&currentFov_, landingFov, defaultFov, 15.0f, Easings::EaseOutExpo);

	// 死亡時のカメラの揺れ
	deadShake_ = std::make_unique<Shake>();
	deadShake_->SetTarget(&shakeOffset_);
	deadShake_->SetRange(deadShakeRange);
	deadShake_->SetEndFrame(deadShakeFrame);
}

void FollowCamera::Init() {
	// 視野角のイージング
	fovAnim_[FovType::kDead].ResetData();
	fovAnim_[FovType::kAir].ResetData();
	fovAnim_[FovType::kLanding].ResetData();

	// 現在の視野角
	currentFov_ = defaultFov;
	camera_->fovY_ = currentFov_;

	// 追従対象との距離
	targetOffset_ = { 0,0,0 };
	
	// 死亡時の揺れ
	deadShake_->Reset();
	deadShake_->SetIsActive(false);
	
	shakeOffset_ = { 0,0,0 };
}

void FollowCamera::Update() {
	// 自機死亡時の演出
	DeadEffect();

	// 視野角
	FovUpdate();

	// 死亡時の揺れ
	deadShake_->Update();

	// 自機のオイラー角を取得
	camera_->transform_.rotate = {
		LerpShortAngle(camera_->transform_.rotate.x, player_->GetWorldTransform()->rotation_.x, rotateRate),
		LerpShortAngle(camera_->transform_.rotate.y, player_->GetWorldTransform()->rotation_.y, rotateRate),
		LerpShortAngle(camera_->transform_.rotate.z, player_->GetWorldTransform()->rotation_.z, rotateRate),
	};

	// 追従対象からカメラまでのオフセット
	Vector3 offset = MathFuncs::TargetOffset(targetOffset_, camera_->GetRotate());
	// 座標をコピーしてオフセット分ずらす
	camera_->transform_.translate = player_->GetWorldTransform()->translation_ + offset + shakeOffset_;

	// カメラの更新
	camera_->Update();
}

void FollowCamera::Release() {
	camera_.reset();
}

void FollowCamera::DebugGui() {
	// ImGui
	if (ImGui::TreeNode("FollowCamera")) {
		ImGui::DragFloat3("Translation", &camera_->transform_.translate.x, 0.1f);
		ImGui::DragFloat3("Rotation", &camera_->transform_.rotate.x, 0.1f);
		ImGui::DragFloat3("offset", &targetOffset_.x, 0.1f, 0, 200);

		if (ImGui::TreeNode("Rate")) {
			ImGui::DragFloat("Rotate", &rotateRate, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Fov")) {
			ImGui::DragFloat("Value", &camera_->fovY_, 0.1f);
			ImGui::DragFloat("Air", &airFov, 0.01f);
			ImGui::DragFloat("Landing", &landingFov, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::Button("Reset")) {
			Init();
		}
		ImGui::TreePop();
	}
}

void FollowCamera::InputUpdate() {
	//if (input_->PushKey(DIK_LEFT)) {
	//	camera_->transform_.rotate.y -= 0.05f;
	//}
	//if (input_->PushKey(DIK_RIGHT)) {
	//	camera_->transform_.rotate.y += 0.05f;
	//}
	//if (input_->PushKey(DIK_DOWN)) {
	//	camera_->transform_.rotate.x -= 0.05f;
	//}
	//if (input_->PushKey(DIK_UP)) {
	//	camera_->transform_.rotate.x += 0.05f;
	//}

	//camera_->transform_.rotate.x = std::clamp<float>(camera_->transform_.rotate.x, -2.0f, 2.0f);
}

void FollowCamera::FovUpdate() {
	// 死亡時の視野角
	if (player_->GetTriggerDead()) {
		fovAnim_[FovType::kAir].ResetData();
		fovAnim_[FovType::kLanding].ResetData();
		fovAnim_[FovType::kDead].SetFirstAnimData(&currentFov_, camera_->fovY_, deadFov, deadShakeFrame, Easings::EaseOutExpo);
		fovAnim_[FovType::kDead].SetIsStart(true);
	}
	else if (player_->GetIsAlive()) {
		// 着地時の視野角
		if (player_->GetTriggerLanding()) {
			fovAnim_[FovType::kDead].ResetData();
			fovAnim_[FovType::kAir].ResetData();
			fovAnim_[FovType::kLanding].SetFirstAnimData(&currentFov_, camera_->fovY_, landingFov, 5.0f, Easings::EaseOutExpo);
			fovAnim_[FovType::kLanding].SetIsStart(true);
		}
		// 空中にいるときの視野角
		else if (player_->GetTriggerAir()) {
			fovAnim_[FovType::kDead].ResetData();
			fovAnim_[FovType::kLanding].ResetData();
			fovAnim_[FovType::kAir].SetFirstAnimData(&currentFov_, camera_->fovY_, airFov, 12.0f, Easings::EaseOutExpo);
			fovAnim_[FovType::kAir].SetIsStart(true);
		}
	}
	fovAnim_[FovType::kDead].Update();
	fovAnim_[FovType::kAir].Update();
	fovAnim_[FovType::kLanding].Update();

	camera_->fovY_ = currentFov_;
}

void FollowCamera::DeadEffect() {
	if (player_->GetIsAlive()) { return; }

	if (deadShake_->GetIsEnd()) {
		deadShake_->Reset();
	}

	// 死亡した瞬間
	if (player_->GetTriggerDead()) {
		deadShake_->SetIsActive(true);
	}
}
