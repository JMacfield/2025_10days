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
	//// カメラの初期化
	//camera = new Camera;
	//camera->Initialize();
	input_ = Input::GetInstance();

	// オーディオのロード
	LoadAudio();

	// 必要なデータの初期化
	InitializeData();
}

// シーン更新関数
void GameScene::Update() {
	
}

// 描画関数
void GameScene::Draw() {

}

// ポストエフェクト描画関数
void GameScene::PostDraw() {
	//postProcess_->Draw();
}

// リソース解放関数
void GameScene::Release() {

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
	ModelManager::GetInstance()->LoadModel("Resources/game", "world.obj");
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