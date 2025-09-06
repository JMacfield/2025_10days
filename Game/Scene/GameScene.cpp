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

GameScene::~GameScene() {}

// 初期化関数
void GameScene::Init() {
	//// カメラの初期化
	input = Input::GetInstance();

	// テクスチャのロード
	LoadTextures();

	// 自機
	player_ = std::make_unique<Player>();
	// テスト壁
	for (int i = 0; i < 5; i++) {
		TestWall* wall = new TestWall();
		wall->Init();
		testWall_.push_back(wall);
	}
	testWall_[0]->SetTranslation(Vector3{ -4.5f,0.0f,0.0f });
	testWall_[1]->SetTranslation(Vector3{ 4.5f,0.0f,4.0f });
	testWall_[2]->SetTranslation(Vector3{ -2.5f,0.0f,8.0f });
	testWall_[3]->SetTranslation(Vector3{ 6.5f,0.0f,12.0f });
	testWall_[4]->SetTranslation(Vector3{ -4.5f,0.0f,16.0f });

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(player_.get());
	followCamera_->Init();
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
	postProcess_ = std::make_unique <PostProcess>();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());

	// モデルのロード
	LoadModels();
	// オーディオのロード
	LoadAudio();

	// 必要なデータの初期化
	InitializeData();


	//Loader::LoadJsonFile2("Resources/game/Json", "Test", objects_, colliders_);
	Loader::LoadJsonFile2("Resources/game/Json", "DemoStage1", objects_, colliders_);
}

// シーン更新関数
void GameScene::Update() {
	for (Object3d* obj : objects_) {
		obj->Update();
	}
	for (auto& obj : objectList_) {
		obj->Update();
	}
	camera->Update();
	camera->Move(1);
	
	objectList_[TENQ]->worldTransform_.rotation_.y += 0.0005f;	// TENQ回転
	
	if (input->TriggerKey(DIK_C)) {
		for (Object3d* obj : objects_) {
			obj->GlitchVerticesLerp(0.8f);
		}
		objectList_[TENQ]->GlitchVerticesLerp(0.08f);
		objectList_[HOLE1]->GlitchVerticesLerp(1.0f);
		objectList_[HOLE2]->GlitchVerticesLerp(5.0f);
		objectList_[HOLE3]->GlitchVerticesLerp(5.0f);
	}
	if (input->TriggerKey(DIK_V)) {
		for (auto& obj : objectList_) {
			obj->StartLerpToOriginalVertices();
		}
		for (Object3d* obj : objects_) {
			obj->StartLerpToOriginalVertices();
		}
	}
	// テスト壁
	for (TestWall* wall : testWall_) {
		wall->Update();
	}
	// 追従カメラ
	followCamera_->Update();
	// 床
	floor_->Update();
	// 自機
	player_->Update();

#ifdef _DEBUG
	ImGui::Begin("GameWindow");
	// 自機
	player_->DebugGui();
	// テスト壁
	//testWall_->DebugGui();
	// 追従カメラ
	followCamera_->DebugGui();
	ImGui::End();
	camera->CameraDebug();
	if (objectList_[TENQ]) objectList_[TENQ]->EasingDebugUI("TENQ");
	if (objectList_[HOLE1]) objectList_[HOLE1]->EasingDebugUI("HoleObject1");
	if (objectList_[HOLE2]) objectList_[HOLE2]->EasingDebugUI("HoleObject2");
	if (objectList_[HOLE3]) objectList_[HOLE3]->EasingDebugUI("HoleObject3");
	objectList_[TENQ]->LightDebug("TENQlight");
	objectList_[HOLE1]->LightDebug("light");
	objectList_[HOLE2]->LightDebug("light2");
	objectList_[HOLE3]->LightDebug("light3");
	objectList_[TENQ]->ModelDebug("TENQmodel");
	objectList_[HOLE1]->ModelDebug("model");
	objectList_[HOLE2]->ModelDebug("model2");
	objectList_[HOLE3]->ModelDebug("model3");

#endif // DEBUG

	// 衝突判定
	collisionManager_->CheckAllCollisions();
	if (input->TriggerKey(DIK_R)) {
		for (auto& obj : objectList_) {
			obj->ResetVerticesToOriginal();
		}
		this->SetSceneNo(CLEARSCENE);
		return;
	}
}

// 描画関数
void GameScene::Draw() {
	// 自機
	player_->Draw();

	for (int i = 0; i < objects_.size(); i++) {
		if (colliders_[i]->GetCollisionAttribute() == kCollisionAttributeEnemy) {
			objects_[i]->Draw(damageWallTex_, followCamera_->GetCamera());
		}
		else {
			objects_[i]->Draw(floorTex_, followCamera_->GetCamera());
		}
	}

	// テスト壁
	for (TestWall* wall : testWall_) {
		//wall->Draw();
	}
	// 床
	floor_->Draw(floorTex_, followCamera_->GetCamera());
	/*TENQ->Draw(textureHandles[TENQ_TEXTURE], followCamera_->GetCamera());
	HoleObject_->Draw(textureHandles[NORMAL_HOLE], followCamera_->GetCamera());*/
	/*HoleObject2_->Draw(textureHandles[NORMAL_HOLE], camera);
	HoleObject3_->Draw(textureHandles[NORMAL_HOLE], camera);*/
}

// ポストエフェクト描画関数
void GameScene::PostDraw() {
	postProcess_->Draw();
}

// リソース解放関数
void GameScene::Release() {
	for (Object3d* obj : objects_) {
		delete obj;
	}
	for (Collider* collider : colliders_) {
		delete collider;
	}
	for (TestWall* wall : testWall_) {
		delete wall;
	}

	collisionManager_->ClearColliderList();
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
	damageWallTex_ = TextureManager::StoreTexture("Resources/red.png");
	//textureHandles[WHITE] = TextureManager::StoreTexture("Resources/white.png");
	textureHandles[NORMAL_HOLE] = TextureManager::StoreTexture("Resources/10days/white.png");
	textureHandles[TENQ_TEXTURE] = TextureManager::StoreTexture("Resources/10days/world.png");
}

// モデルのロード
void GameScene::LoadModels()
{
	ModelManager::GetInstance()->LoadModel("Resources/game", "world.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "Demohole1.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "Demohole2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "Demohole.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "world.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "start.obj");
}

// オーディオのロード
void GameScene::LoadAudio()
{
	//audioHandle[BGM] = Audio::SoundLoadWave("Resources/game/Audio/BGM.wav");
}

// 初期化データのセットアップ
void GameScene::InitializeData() {
	camera = std::make_unique<Camera>();

	const std::array<const char*, 4> modelNames = { "world.obj", "start.obj", "Demohole2.obj", "Demohole.obj" };
	objectList_.clear();
	for (const auto& name : modelNames) {
		auto obj = std::make_unique<Object3d>();
		obj->SetModel(name);
		objectList_.emplace_back(std::move(obj));
	}
	// 自機
	player_ = std::make_unique<Player>();
	// テスト壁
	for (int i = 0; i < 5; i++) {
		TestWall* wall = new TestWall();
		wall->Init();
		testWall_.push_back(wall);
	}
	testWall_[0]->SetTranslation(Vector3{ -4.5f,0.0f,0.0f });
	testWall_[1]->SetTranslation(Vector3{ 4.5f,0.0f,4.0f });
	testWall_[2]->SetTranslation(Vector3{ -2.5f,0.0f,8.0f });
	testWall_[3]->SetTranslation(Vector3{ 6.5f,0.0f,12.0f });
	testWall_[4]->SetTranslation(Vector3{ -4.5f,0.0f,16.0f });

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(player_.get());
	followCamera_->Init();
	player_->SetCamera(followCamera_->GetCamera());
	for (TestWall* wall : testWall_) {
		wall->SetCamera(followCamera_->GetCamera());
	}
	// 床
	floor_ = std::make_unique<Object3d>();
	floor_->Init();
	floor_->SetModel("box.obj");
	floor_->worldTransform_.scale_ = { 10.0f,0.1f,100.0f };
	floor_->worldTransform_.translation_ = { 0.0f,-50.0f,0.0f };

	// ポストエフェクト
	postProcess_ = std::make_unique <PostProcess>();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());
	camera->Initialize();
	for (auto& obj : objectList_) {
		obj->Init();
	}
	objectList_[TENQ]->SetisLight(false);
	objectList_[TENQ]->worldTransform_.scale_ = { -300.0f, 300.0f, 300.0f };
	objectList_[HOLE1]->worldTransform_.scale_ = { 5.0f,5.0f,5.0f };
	objectList_[HOLE3]->worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
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
void GameScene::DrawObjects() {

}

void GameScene::Remake() {

}