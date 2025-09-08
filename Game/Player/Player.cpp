#include "Player.h"
#include "ModelManager.h"
#include "Components/Math/MathFuncs.h"

using namespace MathFuncs;
using namespace PlayerConfig::FileNames;

Player::Player() {
	// 入力の実体を取得
	input_ = Input::GetInstance();
	// ゲームタイマーのインスタンス取得
	gameTimer_ = GameTimer::GetInstance();

	// テクスチャ読み込み
	LoadTextures();

	ModelManager::GetInstance()->LoadModel(Models::body.directoryPath, Models::body.name);

	// 体の実体生成
	body_ = std::make_unique<Object3d>();
	body_->Init();
	body_->SetModel(Models::body.name);
	body_->worldTransform_.translation_ = { -9.1f, 0.0f, 0.0f };

	// colliderの設定
	collider_ = std::make_unique<Collider>();
	collider_->worldTransform.parent_ = &body_->worldTransform_;
	collider_->SetOBBLength(Vector3{ 0.5f, 0.5f,0.5f });
	collider_->SetCollisionPrimitive(kCollisionOBB);
	collider_->SetCollisionAttribute(kCollisionAttributePlayer);
	collider_->SetCollisionMask(~kCollisionAttributePlayer);
	collider_->SetOnCollision(std::bind(&Player::OnCollision, this, std::placeholders::_1));
	collider_->SetIsActive(true);

	// 全ての機能生成
	CreateSystems();

	currentWallSide_ = WallSide::kNone;
	// 今の次元
	currentDimension_ = DimensionType::kNow;
}

void Player::Init() {
	currentDimension_ = DimensionType::kNow;
	body_->SetTexture(Textures::body.num);
	body_->worldTransform_.translation_ = { -9.1f, 0.0f, 0.0f };
	body_->worldTransform_.rotation_ = { 0,0,0 };
	body_->worldTransform_.scale_ = { 1,1,1 };
	body_->Update();

	moveSystem_->Init();
	jumpSystem_->Init();

	// 空中にいるか
	isAir_ = true;
	// 着地したか
	isLanding_ = false;
	isAlive_ = true;
	isClear_ = false;
	switchDimension_ = false;

	vel_ = { 0.0f,0.0f,0.0f };
	rot_ = { 0.0f,0.0f,0.0f };

	currentWallSide_ = WallSide::kNone;
}

void Player::Update() {
	isPreAlive_ = isAlive_;
	preDimension_ = currentDimension_;
	switchDimension_ = false;
	if (!isAlive_) { return; }

	isPreAir_ = isAir_;

	// 過去現在の切り替え
	if (input_->TriggerKey(PlayerConfig::Input::Keyboard::switching)) {
		switchDimension_ = true;
		if (currentDimension_ == DimensionType::kNow) {
			currentDimension_ = DimensionType::kPast;
		}
		else if (currentDimension_ == DimensionType::kPast) {
			currentDimension_ = DimensionType::kNow;
		}
	}
	else if (input_->GetJoystickState(joyState)) {
		if (PlayerConfig::Input::GamePad::GamePadTrigger(joyState, preJoyState, XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			switchDimension_ = true;
			if (currentDimension_ == DimensionType::kNow) {
				currentDimension_ = DimensionType::kPast;
			}
			else if (currentDimension_ == DimensionType::kPast) {
				currentDimension_ = DimensionType::kNow;
			}
		}
	}

	// 移動処理
	moveSystem_->Update();
	// ジャンプ
	jumpSystem_->Update();

	// 速度
	Vector3 vel = moveSystem_->GetVel() + jumpSystem_->GetVel() + vel_;
	body_->worldTransform_.translation_ += vel;

	// 壁に着地時の角度
	LandingRotate();
	// 角度
	Vector3 rot = moveSystem_->GetRot() + rot_;
	body_->worldTransform_.rotation_ = rot;

	// 体
	body_->Update();

	// 何も接触していないときは落下する
	if (!collider_->GetIsOnCollision()) {
		// 空中にいるか
		isAir_ = true;
		// 着地したか
		isLanding_ = false;
	}

	if (isAir_ && !isLanding_) {
		vel_.y -= acceleration * GameTimer::GetInstance()->GetTimeScale();
	}

	preJoyState = joyState;
}

void Player::Draw() {
	// 体
	body_->Draw(pCamera_);
}

void Player::Release() {
	// 体
	body_.reset();
	// 移動
	moveSystem_.reset();
	// ジャンプ
	jumpSystem_.reset();
}

void Player::DebugGui() {
	if (ImGui::TreeNode("Player")) {
		if (ImGui::TreeNode("Object")) {
			ImGui::DragFloat3("Translation", &body_->worldTransform_.translation_.x, 0.1f, -1000.0f, 1000.0f);
			ImGui::DragFloat3("Rotation", &body_->worldTransform_.rotation_.x, 0.01f, -6.28f, 6.28f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Jump")) {
			jumpSystem_->DebugGui();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Move")) {
			moveSystem_->DebugGui();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Landing")) {
			ImGui::DragFloat3("Rotate", &landingRot.x, 0.01f, -6.28f, 6.28f);
			ImGui::DragFloat("OffsetX", &landingOffsetX, 0.1f, -100.0f, 100.0f);
			ImGui::TreePop();
		}
		int a = (int)currentWallSide_;
		ImGui::DragFloat("Acceleration", &acceleration, 0.0001f, -100.0f, 100.0f);
		ImGui::DragInt("WallSide", &a);
		ImGui::Checkbox("IsAir", &isAir_);
		ImGui::Checkbox("IsLanding", &isLanding_);
		ImGui::Checkbox("IsAlive", &isAlive_);

		if (ImGui::Button("Stop")) {
			vel_ = { 0.0f,0.0f,0.0f };
			acceleration = 0.0f;
			moveSystem_->maxMoveVel = { 0.0f,0.0f,0.0f };
		}
		if (ImGui::Button("Reset")) {
			Init();
		}
		ImGui::TreePop();
	}
}

void Player::LoadTextures() {
	// 体
	Textures::body.num = TextureManager::StoreTexture(Textures::body.name);
}

void Player::CreateSystems() {
	// 移動処理
	moveSystem_ = std::make_unique<MoveSystem>();
	moveSystem_->Init();

	// ジャンプ処理
	jumpSystem_ = std::make_unique<JumpSystem>(this);
	jumpSystem_->Init();
}

void Player::OnCollision(Collider* collider) {
	// 攻撃に当たったら死亡
	if (collider->GetCollisionAttribute() == kCollisionAttributeEnemy) {
		isAlive_ = false;
	}
	else if (collider->GetCollisionAttribute() == kCollisionAttributeClear) {
		isClear_ = true;
	}
	// 壁の場合
	else if (collider->GetCollisionAttribute() == kCollisionAttributeObstacles) {
		// 着地の判定
		CheckLanding(collider);
	}
}

void Player::CheckLanding(Collider* collider) {
	// 空中に飛んでいなければ処理しない
	if (!isAir_) { return; }

	// 着地時の座標補間
	Vector3 a = MathFuncs::GetWorldPosition(collider->worldTransform.matWorld_) - MathFuncs::GetWorldPosition(body_->worldTransform_.matWorld_);
	a = a.Normalize();
	// 壁が左側にある
	if (a.x >= 0.0f) {
		if (currentWallSide_ == WallSide::kLeft || currentWallSide_ == WallSide::kNone) {
			currentWallSide_ = WallSide::kRight;
			// 空中にいるか
			isAir_ = false;
			// 着地したか
			isLanding_ = true;
			vel_ = { 0.0f,0.0f,0.0f };

			// 座標の補間
			//float posX = MathFuncs::GetWorldPosition(collider->worldTransform.matWorld_).x - landingOffsetX;
			float posX = MathFuncs::GetWorldPosition(collider->worldTransform.matWorld_).x - collider->GetOBB().m_fLength.x - landingOffsetX;
			body_->worldTransform_.translation_.x = posX;

			// ジャンプ状態を解除
			jumpSystem_->Init();
		}
	}
	// 壁が右側にある
	else if (a.x < 0.0f) {
		if (currentWallSide_ == WallSide::kRight || currentWallSide_ == WallSide::kNone) {
			currentWallSide_ = WallSide::kLeft;
			// 空中にいるか
			isAir_ = false;
			// 着地したか
			isLanding_ = true;
			vel_ = { 0.0f,0.0f,0.0f };

			// 座標の補間
			//float posX = MathFuncs::GetWorldPosition(collider->worldTransform.matWorld_).x + landingOffsetX;
			float posX = MathFuncs::GetWorldPosition(collider->worldTransform.matWorld_).x + collider->GetOBB().m_fLength.x + landingOffsetX;
			body_->worldTransform_.translation_.x = posX;

			// ジャンプ状態を解除
			jumpSystem_->Init();
		}
	}

}

void Player::LandingRotate() {
	// 空中にいるとき
	if (isAir_) {
		rot_ = Lerps::ExponentialInterpolate(rot_, Vector3{ 0,0,0 }, 0.05f * GameTimer::GetInstance()->GetTimeScale());
	}
	// 壁が右側にある
	else if (currentWallSide_ == WallSide::kLeft) {
		Vector3 goalRot = landingRot;
		goalRot.z *= -1;
		rot_ = Lerps::ExponentialInterpolate(rot_, goalRot, 0.1f * GameTimer::GetInstance()->GetTimeScale());
	}
	// 壁が左側にある
	else if (currentWallSide_ == WallSide::kRight) {
		Vector3 goalRot = landingRot;
		rot_ = Lerps::ExponentialInterpolate(rot_, goalRot, 0.1f * GameTimer::GetInstance()->GetTimeScale());
	}
}

void Player::StartJump() {
	// 空中にいるか
	isAir_ = true;
	// 着地したか
	isLanding_ = false;
	vel_ = { 0.0f,0.0f,0.0f };
}

void Player::EndJump() {
	// 壁を走っている状態なら処理しない
	if (!isAir_ && isLanding_) { return; }

	// 壁が右側にある
	if (currentWallSide_ == WallSide::kLeft) {
		currentWallSide_ = WallSide::kRight;
	}
	// 壁が左側にある
	else if (currentWallSide_ == WallSide::kRight) {
		currentWallSide_ = WallSide::kLeft;
	}
	// 空中にいるか
	isAir_ = false;
	// 着地したか
	isLanding_ = true;
	vel_ = { 0.0f,0.0f,0.0f };
}
