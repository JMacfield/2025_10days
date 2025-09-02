#include "FollowCamera.h"
#include "../Player/Math/MathFuncs.h"
#include "../Player/Player.h"

FollowCamera::FollowCamera(Player* player) {
	player_ = player;
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
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	//camera_->SetViewMatrix(Inverse(camera_->worldTransform_.matWorld_));

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

}
