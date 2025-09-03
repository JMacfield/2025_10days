#include "FollowCamera.h"
#include "../Player/Math/MathFuncs.h"
#include "../Player/Player.h"

FollowCamera::FollowCamera(Player* player) {
	player_ = player;

	input_ = Input::GetInstance();

	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
}

void FollowCamera::Init() {

}

void FollowCamera::Update() {
	// 自機のオイラー角を取得
	camera_->transform_.rotate = player_->GetWorldTransform()->rotation_;

	// 追従対象からカメラまでのオフセット
	Vector3 offset = MathFuncs::TargetOffset(targetOffset_, camera_->GetRotate());

	// 座標をコピーしてオフセット分ずらす
	camera_->transform_.translate = player_->GetWorldTransform()->translation_ + offset;

	// カメラの更新
	camera_->Update();
#ifdef _DEBUG
	// ImGui
	ImGui::Begin("FollowCamera");
	ImGui::DragFloat3("translation", &camera_->transform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotation", &camera_->transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("offset", &targetOffset_.x, 0.1f, 0, 200);
	ImGui::End();
#endif // _DEBUG
}

void FollowCamera::Draw() {

}

void FollowCamera::Release() {
	camera_.reset();
}

void FollowCamera::DebugGui() {
	// ImGui
	if (ImGui::TreeNode("FollowCamera")) {
		ImGui::DragFloat3("translation", &camera_->transform_.translate.x, 0.1f);
		ImGui::DragFloat3("rotation", &camera_->transform_.rotate.x, 0.1f);
		ImGui::DragFloat3("offset", &targetOffset_.x, 0.1f, 0, 200);
		ImGui::TreePop();
	}
}

void FollowCamera::InputUpdate() {
	if (input_->PushKey(DIK_LEFT)) {
		camera_->transform_.rotate.y -= 0.05f;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		camera_->transform_.rotate.y += 0.05f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		camera_->transform_.rotate.x -= 0.05f;
	}
	if (input_->PushKey(DIK_UP)) {
		camera_->transform_.rotate.x += 0.05f;
	}

	camera_->transform_.rotate.x = std::clamp<float>(camera_->transform_.rotate.x, -2.0f, 2.0f);
}
