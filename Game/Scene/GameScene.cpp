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

GameScene::GameScene(){}

GameScene::~GameScene() {
	Release();
}

// 初期化関数
void GameScene::Init() {
	input_ = Input::GetInstance();

	// 自機
	player_ = std::make_unique<Player>();
	// テスト壁
	testWall_ = std::make_unique<TestWall>();
	testWall_->Init();

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(player_.get());
	player_->SetCamera(followCamera_->GetCamera());
	testWall_->SetCamera(followCamera_->GetCamera());

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
	testWall_->Update();
	// 自機
	player_->Update();
	// 追従カメラ
	followCamera_->Update();

#ifdef _DEBUG
	ImGui::Begin("GameWindow");
	// 自機
	player_->DebugGui();
	// テスト壁
	testWall_->DebugGui();
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
	testWall_->Draw();
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
	//textureHandles[WHITE] = TextureManager::StoreTexture("Resources/white.png");
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