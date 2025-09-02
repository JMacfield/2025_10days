#include "GameScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "mathFunction.h"
#include "PSOPostEffect.h"
#include "Loader.h"
#include "Audio.h"
#include <iostream>
#include <cmath>
#include <DirectXMath.h>

GameScene::GameScene() {
	collisionManager_ = CollisionManager::GetInstance();
}

GameScene::~GameScene() {
	Release();

	collisionManager_->ClearColliderList();

	for (TestWall* wall : testWall_) {
		delete wall;
	}
}

// 初期化関数
void GameScene::Init() {
	input_ = Input::GetInstance();

	LoadTextures();

	// 自機
	player_ = std::make_unique<Player>();
	// テスト壁
	for (int i = 0; i < 5;i++) {
		TestWall* wall = new TestWall();
		wall->Init();
		testWall_.push_back(wall);
	}
	testWall_[0]->SetTranslation(Vector3{ -4.5f,0.0f,0.0f });
	testWall_[1]->SetTranslation(Vector3{ 4.5f,0.0f,4.0f });
	testWall_[2]->SetTranslation(Vector3{ -4.5f,0.0f,8.0f });
	testWall_[3]->SetTranslation(Vector3{ 4.5f,0.0f,12.0f });
	testWall_[4]->SetTranslation(Vector3{ -4.5f,0.0f,16.0f });

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(player_.get());
	player_->SetCamera(followCamera_->GetCamera());
	for (TestWall* wall : testWall_) {
		wall->SetCamera(followCamera_->GetCamera());
	}

	// 床
	floor_ = std::make_unique<Object3d>();
	floor_->Init();
	floor_->SetModel("box.obj");
	floor_->worldTransform_.scale_ = { 10.0f,0.1f,100.0f };
	floor_->worldTransform_.translation_ = { 0.0f,-1.0f,0.0f };

	// ポストエフェクト
	postProcess_ = new PostProcess();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());

	// オーディオのロード
	LoadAudio();

	// 必要なデータの初期化
	InitializeData();
}

// シーン更新関数
void GameScene::Update() {
	// テスト壁
	for (TestWall* wall : testWall_) {
		wall->Update();
	}
	// 床
	floor_->Update();
	// 自機
	player_->Update();
	// 追従カメラ
	followCamera_->Update();

	// 衝突判定
	collisionManager_->CheckAllCollisions();
#ifdef _DEBUG
	ImGui::Begin("GameWindow");
	// 自機
	player_->DebugGui();
	// テスト壁
	//testWall_->DebugGui();
	// 追従カメラ
	followCamera_->DebugGui();
	ImGui::End();
#endif // DEBUG
}

// 描画関数
void GameScene::Draw() {
	// 自機
	player_->Draw();
	// テスト壁
	for (TestWall* wall : testWall_) {
		wall->Draw();
	}
	// 床
	floor_->Draw(floorTex_, followCamera_->GetCamera());
}

// ポストエフェクト描画関数
void GameScene::PostDraw() {
	postProcess_->Draw();
}

// リソース解放関数
void GameScene::Release() {
	//delete camera_;
}

// ゲーム終了判定関数
int GameScene::GameClose()
{
	return false;
}

///Refactoring///

///Init///
// テクスチャのロード
void GameScene::LoadTextures()
{
	floorTex_ = TextureManager::StoreTexture("Resources/white.png");
}

// モデルのロード
void GameScene::LoadModels()
{
	//ModelManager::GetInstance()->LoadModel("Resources/game", "world.obj");
}

// オーディオのロード
void GameScene::LoadAudio()
{
	//audioHandle[BGM] = Audio::SoundLoadWave("Resources/game/Audio/BGM.wav");
}

// 初期化データのセットアップ
void GameScene::InitializeData() {

}

// ゲームパッド入力処理
void GameScene::HandleGamePadInput() {
	//XINPUT_STATE joyState;
	//if (Input::GetInstance()->GetJoystickState(joyState)) {
	//	HandleStartButton(joyState);
	//	if (isMenu) HandleMenuNavigation(joyState);
	//	else UpdateCameraFOV(joyState);
	//}
}

///Draw///
// オブジェクトの描画
void GameScene::DrawObjects()
{
}

void GameScene::Remake() {

}