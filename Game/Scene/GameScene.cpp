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
	sceneNo = GAMESCENE;

	//// カメラの初期化
	input = Input::GetInstance();

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
	/*if (!player_->GetIsAlive()) {
		this->SetSceneNo(OVERSCENE);
		return;
	}
	else if (player_->GetIsClear()) {
		this->SetSceneNo(CLEARSCENE);
		return;
	}*/

	/*if (input->TriggerKey(DIK_Z)) {
		this->SetSceneNo(CLEARSCENE);
	}*/

	ShowDeathUI();

	obstacleManager_->Update();
	for (auto& obj : objectList_) {
		obj->Update();
	}
	camera->Update();
	camera->Move(1);

	objectList_[TENQ]->worldTransform_.rotation_.y += 0.0005f;	// TENQ回転

	// 追従カメラ
	followCamera_->Update();
	// 床
	//floor_->Update();
	// 自機
	player_->Update();

	uiSprite_[UI_TEXTURE]->Update();
	uiSprite_[UI_TEXTURE2]->Update();
	uiSprite_[UI_TEXTURE3]->Update();

	overSprite_->Update();
	showContinue_->Update();

#ifdef _DEBUG
	ImGui::Begin("GameWindow");
	// 自機
	player_->DebugGui();
	// 追従カメラ
	followCamera_->DebugGui();
	GameTimer::GetInstance()->Update();
	ImGui::End();
	camera->CameraDebug();
	if (objectList_[TENQ]) objectList_[TENQ]->EasingDebugUI("TENQ");
	if (objectList_[RULE]) objectList_[RULE]->EasingDebugUI("Rule");

#endif // DEBUG

	// 衝突判定
	collisionManager_->CheckAllCollisions();

	if (input->TriggerKey(DIK_R)) {
		for (auto& obj : objectList_) {
			obj->ResetVerticesToOriginal();
		}
		/*for (auto& obj : objects_) {
			obj->ResetVerticesToOriginal();
		}*/
		this->SetSceneNo(CLEARSCENE);
		this->SetSceneNo(TITLESCENE);
		return;
	}
}

// 描画関数
void GameScene::Draw() {
	if (!player_->GetIsAlive()) {
		overSprite_->Draw(overSpriteHandle_, { 1.0f,1.0f,1.0f,1.0f });
		showContinue_->Draw(showContinueHandle_, { 1.0f,1.0f,1.0f,1.0f });
	}

	// 自機
	player_->Draw();

	obstacleManager_->Draw();

	// テスト壁
	for (TestWall* wall : testWall_) {
		//wall->Draw();
	}
	for (auto& obj : objectList_) {
		obj->Draw(followCamera_->GetCamera());
	}
	// 床
	//floor_->Draw(followCamera_->GetCamera());

	uiSprite_[UI_TEXTURE]->Draw(uiTextureHandles_[UI_TEXTURE], { 1.0f,1.0f,1.0f,1.0f });
	uiSprite_[UI_TEXTURE2]->Draw(uiTextureHandles_[UI_TEXTURE2], { 1.0f,1.0f,1.0f,1.0f });
	uiSprite_[UI_TEXTURE3]->Draw(uiTextureHandles_[UI_TEXTURE3], { 1.0f,1.0f,1.0f,1.0f });
}

// ポストエフェクト描画関数
void GameScene::PostDraw() {
	postProcess_->Draw();
}

// リソース解放関数
void GameScene::Release() {
	//for (Object3d* obj : objects_) {
	//	delete obj;
	//}
	//for (Collider* collider : colliders_) {
	//	delete collider;
	//}
	/*for (TestWall* wall : testWall_) {
		delete wall;
	}*/

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
	floorTex_ = TextureManager::StoreTexture("Resources/game/white.png");
	damageWallTex_ = TextureManager::StoreTexture("Resources/red.png");
	greenWallTex_ = TextureManager::StoreTexture("Resources/green.png");
	//textureHandles[WHITE] = TextureManager::StoreTexture("Resources/white.png");
	textureHandles[NORMAL_HOLE] = TextureManager::StoreTexture("Resources/game/Rule.png");
	textureHandles[TENQ_TEXTURE] = TextureManager::StoreTexture("Resources/10days/world.png");

	uiTextureHandles_[UI_TEXTURE] = TextureManager::GetInstance()->StoreTexture("Resources/ui/ui_jump.png");
	uiTextureHandles_[UI_TEXTURE2] = TextureManager::GetInstance()->StoreTexture("Resources/ui/ui_switch.png");
	uiTextureHandles_[UI_TEXTURE3] = TextureManager::GetInstance()->StoreTexture("Resources/ui/ui_camera.png");

	overSpriteHandle_ = TextureManager::GetInstance()->StoreTexture("Resources/gameover/gameover.png");

	showContinueHandle_ = TextureManager::GetInstance()->StoreTexture("Resources/gameover/ui_continue.png");
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
	ModelManager::GetInstance()->LoadModel("Resources/game/", "Rule.obj");
}

// オーディオのロード
void GameScene::LoadAudio()
{
	//audioHandle[BGM] = Audio::SoundLoadWave("Resources/game/Audio/BGM.wav");
}

// 初期化データのセットアップ
void GameScene::InitializeData() {
	camera = std::make_unique<Camera>();
	const std::array<const char*, 2> modelNames = { "world.obj", "Rule.obj"};
	objectList_.clear();
	for (const auto& name : modelNames) {
		auto obj = std::make_unique<Object3d>();
		obj->Init();
		obj->SetisLight(false);
		obj->SetModel(name);
		objectList_.emplace_back(std::move(obj));
	}

	objectList_[TENQ]->SetTexture(textureHandles[TENQ_TEXTURE]);
	objectList_[RULE]->SetTexture(textureHandles[NORMAL_HOLE]);
	/*for (int i = 0; i < objects_.size(); i++) {
		if (colliders_[i]->GetCollisionAttribute() == kCollisionAttributeEnemy) {
			objects_[i]->SetTexture(damageWallTex_);
		}
		else {
			objects_[i]->SetTexture(floorTex_);
		}
	}*/


	// 自機
	player_ = std::make_unique<Player>();

	// テスト壁
	//for (int i = 0; i < 5; i++) {
	//	TestWall* wall = new TestWall();
	//	wall->Init();
	//	testWall_.push_back(wall);
	//}
	//testWall_[0]->SetTranslation(Vector3{ -4.5f,0.0f,0.0f });
	//testWall_[1]->SetTranslation(Vector3{ 4.5f,0.0f,4.0f });
	//testWall_[2]->SetTranslation(Vector3{ -2.5f,0.0f,8.0f });
	//testWall_[3]->SetTranslation(Vector3{ 6.5f,0.0f,12.0f });
	//testWall_[4]->SetTranslation(Vector3{ -4.5f,0.0f,16.0f });

	//// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(player_.get());
	followCamera_->Init();
	player_->SetCamera(followCamera_->GetCamera());
	for (TestWall* wall : testWall_) {
		wall->SetCamera(followCamera_->GetCamera());
	}
	// 床
	/*floor_ = std::make_unique<Object3d>();
	floor_->Init();
	floor_->SetModel("box.obj");
	floor_->SetTexture(floorTex_);
	floor_->worldTransform_.scale_ = { 10.0f,0.1f,100.0f };
	floor_->worldTransform_.translation_ = { 0.0f,-50.0f,0.0f };*/

	// ポストエフェクト
	postProcess_ = std::make_unique <PostProcess>();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());
	camera->Initialize();

	// 障害物の管理クラス
	obstacleManager_ = std::make_unique<ObstacleManager>(followCamera_->GetCamera(), player_.get());
	obstacleManager_->Init();

	uiSprite_[UI_TEXTURE] = std::make_unique<Sprite>();
	uiSprite_[UI_TEXTURE2] = std::make_unique<Sprite>();
	uiSprite_[UI_TEXTURE3] = std::make_unique<Sprite>();

	uiSprite_[UI_TEXTURE]->Init({ 980.0f,620.0f }, { 300.0f,100.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/ui/ui_jump.png");
	uiSprite_[UI_TEXTURE2]->Init({ 980.0f,565.0f }, { 300.0f,100.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/ui/ui_switch.png");
	uiSprite_[UI_TEXTURE3]->Init({ 980.0f,515.0f }, { 300.0f,100.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/ui/ui_camera.png");

	uiSprite_[UI_TEXTURE]->SetTextureSize({ 300.0f,150.0f });
	uiSprite_[UI_TEXTURE2]->SetTextureSize({ 300.0f,150.0f });
	uiSprite_[UI_TEXTURE3]->SetTextureSize({ 300.0f,150.0f });

	overSprite_ = std::make_unique<Sprite>();
	overSprite_->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/gameover/gameover.png");
	overSprite_->SetTextureSize({ 1280.0f,720.0f });

	showContinue_ = std::make_unique<Sprite>();
	showContinue_->Init({ 390.0f,370.0f }, { 500.0f,350.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/gameover/ui_continue.png");
	showContinue_->SetTextureSize({ 300.0f,150.0f });
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

void GameScene::ShowDeathUI() {
	if (!player_->GetIsAlive()) {
		if (input->TriggerButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_SPACE)) {
			// ここにリセット処理
			player_->Reset();
			obstacleManager_->Init();
			//
		}
		if (input->TriggerButton(XINPUT_GAMEPAD_B)|| input->TriggerKey(DIK_RETURN)) {
			this->SetSceneNo(TITLESCENE);
		}
	}
	else if (player_->GetIsClear()) {
		this->SetSceneNo(CLEARSCENE);
		return;
	}
}