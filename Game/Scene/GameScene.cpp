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
	////// カメラの初期化
	//input = Input::GetInstance();
	//

	//// テクスチャのロード
	//LoadTextures();

	//// 自機
	//player_ = std::make_unique<Player>();
	//// テスト壁
	//for (int i = 0; i < 5;i++) {
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
	//followCamera_ = std::make_unique<FollowCamera>(player_.get());
	//followCamera_->Init();
	//player_->SetCamera(followCamera_->GetCamera());
	//for (TestWall* wall : testWall_) {
	//	wall->SetCamera(followCamera_->GetCamera());
	//}

	//// 床
	//floor_ = std::make_unique<Object3d>();
	//floor_->Init();
	//floor_->SetModel("box.obj");
	//floor_->worldTransform_.scale_ = { 10.0f,0.1f,100.0f };
	//floor_->worldTransform_.translation_ = { 0.0f,-1.0f,0.0f };

	//// ポストエフェクト
	//postProcess_ = std::make_unique <PostProcess>();
	//postProcess_->Init();
	//postProcess_->SetCamera(followCamera_->GetCamera());

	//// オーディオのロード
	//LoadAudio();

	//// 必要なデータの初期化
	//InitializeData();


}

// シーン更新関数
void GameScene::Update() {
//	camera->Update();		
//	camera->Move(1);
//	TENQ->Update();
//	HoleObject_->Update();
//	HoleObject2_->Update();
//	HoleObject3_->Update();
//	
//	TENQ->worldTransform_.rotation_.y += 0.0005f;
//	if (input->TriggerKey(DIK_SPACE)) {
//		TENQ->GlitchVerticesLerp(0.08f);
//		HoleObject_->GlitchVerticesLerp(1.0f);
//		HoleObject2_->GlitchVerticesLerp(5.0f);
//		HoleObject3_->GlitchVerticesLerp(5.0f);
//	}
//	if (input->TriggerKey(DIK_V)) {
//		TENQ->StartLerpToOriginalVertices();
//		HoleObject_->StartLerpToOriginalVertices();
//		HoleObject2_->StartLerpToOriginalVertices();
//		HoleObject3_->StartLerpToOriginalVertices();
//	}
//	camera->CameraDebug();
//	if (TENQ) TENQ->EasingDebugUI("TENQ");
//	if (HoleObject_) HoleObject_->EasingDebugUI("HoleObject1");
//	if (HoleObject2_) HoleObject2_->EasingDebugUI("HoleObject2");
//	if (HoleObject3_) HoleObject3_->EasingDebugUI("HoleObject3");
//	TENQ->LightDebug("TENQlight");
//	HoleObject_->LightDebug("light");
//	HoleObject2_->LightDebug("light2");
//	HoleObject3_->LightDebug("light3");
//	TENQ->ModelDebug("TENQmodel");
//	HoleObject_->ModelDebug("model");
//	HoleObject2_->ModelDebug("model2");
//	HoleObject3_->ModelDebug("model3");
	if (Input::GetInstance()->PushKey(DIK_R)) {
		this->SetSceneNo(CLEARSCENE);
		return;
	}
//	// テスト壁
//	for (TestWall* wall : testWall_) {
//		wall->Update();
//	}
//	// 追従カメラ
//	followCamera_->Update();
//	// 床
//	floor_->Update();
//	// 自機
//	player_->Update();
//
//	// 衝突判定
//	collisionManager_->CheckAllCollisions();
//#ifdef _DEBUG
//	ImGui::Begin("GameWindow");
//	// 自機
//	player_->DebugGui();
//	// テスト壁
//	//testWall_->DebugGui();
//	// 追従カメラ
//	followCamera_->DebugGui();
//	ImGui::End();
//#endif // DEBUG
}


// 描画関数
void GameScene::Draw() {
	//// 自機
	//player_->Draw();
	//// テスト壁
	//for (TestWall* wall : testWall_) {
	//	wall->Draw();
	//}
	//// 床
	//floor_->Draw(floorTex_, followCamera_->GetCamera());
	//TENQ->Draw(textureHandles[TENQ_TEXTURE], followCamera_->GetCamera());
	//HoleObject_->Draw(textureHandles[NORMAL_HOLE], followCamera_->GetCamera());
	/*HoleObject2_->Draw(textureHandles[NORMAL_HOLE], camera);
	HoleObject3_->Draw(textureHandles[NORMAL_HOLE], camera);*/
}

// ポストエフェクト描画関数
void GameScene::PostDraw() {
	//postProcess_->Draw();
}

// リソース解放関数
void GameScene::Release() {
	//delete camera_;
	/*for (TestWall* wall : testWall_) {
		delete wall;
	}
	collisionManager_->ClearColliderList();*/

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
	//textureHandles[WHITE] = TextureManager::StoreTexture("Resources/white.png");
	textureHandles[NORMAL_HOLE] = TextureManager::StoreTexture("Resources/10days/white.png");
	textureHandles[TENQ_TEXTURE] = TextureManager::StoreTexture("Resources/10days/world.png");

}

// モデルのロード
void GameScene::LoadModels()
{
	//ModelManager::GetInstance()->LoadModel("Resources/game", "world.obj");
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
void GameScene::InitializeData(){
	camera = std::make_unique<Camera>();
	TENQ = std::make_unique<Object3d>();
	HoleObject_ = std::make_unique<Object3d>();
	HoleObject2_ = std::make_unique<Object3d>();
	HoleObject3_ = std::make_unique<Object3d>();
	postProcess_ = std::make_unique<PostProcess>();

	camera->Initialize();
	TENQ->Init();
	HoleObject_->Init();
	HoleObject2_->Init();
	HoleObject3_->Init();
	postProcess_->Init();
	postProcess_->SetCamera(camera.get());
	TENQ->SetModel("world.obj");
	TENQ->SetisLight(false);
	TENQ->worldTransform_.scale_ = { -300.0f, 300.0f, 300.0f };
	HoleObject_->SetModel("start.obj");
	HoleObject2_->SetModel("Demohole2.obj");
	HoleObject3_->SetModel("Demohole.obj");
	HoleObject_->worldTransform_.scale_ = { 5.0f,5.0f,5.0f };
	HoleObject3_->worldTransform_.scale_ = { 0.5f,0.5f,0.5f };	 
	camera->transform_.translate = { -0.191f,-41.0f,-466.0f };
	camera->transform_.rotate = { -0.26f,-0.060f,0.0f };
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