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
	camera = new Camera;
	camera->Initialize();
	//input = Input::GetInstance();
	HoleObject_ = new Object3d();
	HoleObject_->Init();
	HoleObject_->SetModel("Demohole.obj");


	// テクスチャのロード
	LoadTextures();

	// モデルのロード
	LoadModels();

	// オーディオのロード
	LoadAudio();

	// 必要なデータの初期化
	InitializeData();
}

// シーン更新関数
void GameScene::Update() {
	HoleObject_->Update();
	
}

// 描画関数
void GameScene::Draw() {
	HoleObject_->Draw(textureHandles[NORMAL_HOLE], camera);
}

// ポストエフェクト描画関数
void GameScene::PostDraw()
{
	//postProcess_->Draw();
}

// リソース解放関数
void GameScene::Release() {
	int a;
	a = 10;
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
	textureHandles[NORMAL_HOLE] = TextureManager::StoreTexture("Resources/10days/white.png");
}

// モデルのロード
void GameScene::LoadModels()
{
	//ModelManager::GetInstance()->LoadModel("Resources/game", "world.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "Demohole.obj");

}

// オーディオのロード
void GameScene::LoadAudio()
{
	//audioHandle[BGM] = Audio::SoundLoadWave("Resources/game/Audio/BGM.wav");
}

// 初期化データのセットアップ
void GameScene::InitializeData()
{

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
	//for (auto& cone : ConeObject_) {
	//	if (cone->isVisible) {
	//		cone->Draw(textureHandles[CONE], camera);
	//	}
	//}
	//for (auto& star : StarObject_) {
	//	if (star->isVisible) {
	//		star->Draw(textureHandles[STAR], camera);
	//	}
	//}
	//for (auto& item : ItemObject_) {
	//	if (item->isVisible) {
	//		item->Draw(textureHandles[ITEM], camera);
	//	}
	//}
	//flowEffect_.Draw(textureHandles[GRID], camera);
}

void GameScene::Remake() {

}