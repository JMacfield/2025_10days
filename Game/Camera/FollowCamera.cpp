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

	fovAnim_[FovType::kAir].SetAnimData(&currentFov_, 0.0f, airFov, 12.0f, Easings::EaseOutExpo);

	fovAnim_[FovType::kLanding].SetAnimData(&currentFov_, 0.0f, landingFov, 5.0f, Easings::EaseOutExpo);
	fovAnim_[FovType::kLanding].SetAnimData(&currentFov_, landingFov, defaultFov, 15.0f, Easings::EaseOutExpo);
}

void FollowCamera::Init() {

}

void FollowCamera::Update() {
	// 視野角
	FovUpdate();

	// 自機のオイラー角を取得
	camera_->transform_.rotate = {
		LerpShortAngle(camera_->transform_.rotate.x, player_->GetWorldTransform()->rotation_.x, rotateRate),
		LerpShortAngle(camera_->transform_.rotate.y, player_->GetWorldTransform()->rotation_.y, rotateRate),
		LerpShortAngle(camera_->transform_.rotate.z, player_->GetWorldTransform()->rotation_.z, rotateRate),
	};

	// 追従対象からカメラまでのオフセット
	Vector3 offset = MathFuncs::TargetOffset(targetOffset_, camera_->GetRotate());
	// 座標をコピーしてオフセット分ずらす
	camera_->transform_.translate = player_->GetWorldTransform()->translation_ + offset;

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
		ImGui::DragFloat("RotateRate", &rotateRate, 0.01f);
		if (ImGui::TreeNode("Fov")) {
			ImGui::DragFloat("Value", &camera_->fovY_, 0.1f);
			ImGui::DragFloat("Air", &airFov, 0.01f);
			ImGui::DragFloat("Landing", &landingFov, 0.01f);
			ImGui::TreePop();
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
	// 着地時の視野角
	if (player_->GetTriggerLanding()) {
		fovAnim_[FovType::kAir].ResetData();
		fovAnim_[FovType::kLanding].SetFirstAnimData(&currentFov_, camera_->fovY_, landingFov, 5.0f, Easings::EaseOutExpo);
		fovAnim_[FovType::kLanding].SetIsStart(true);
	}
	// 空中にいるときの視野角
	else if (player_->GetTriggerAir()) {
		fovAnim_[FovType::kLanding].ResetData();
		fovAnim_[FovType::kAir].SetFirstAnimData(&currentFov_, camera_->fovY_, airFov, 12.0f, Easings::EaseOutExpo);
		fovAnim_[FovType::kAir].SetIsStart(true);
	}
	fovAnim_[FovType::kAir].Update();
	fovAnim_[FovType::kLanding].Update();

	camera_->fovY_ = currentFov_;
}