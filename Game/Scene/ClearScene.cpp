#include "ClearScene.h"
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

ClearScene::ClearScene() {}
ClearScene::~ClearScene() {}

void ClearScene::Init() {
	sceneNo = CLEARSCENE;

	// 初期化処理
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
void ClearScene::Update() {
	// 更新処理
	camera->Update();
	camera->Move(1);
	TENQ->Update();
	HoleObject_->Update();
	HoleObject2_->Update();
	HoleObject3_->Update();

	TENQ->worldTransform_.rotation_.y += 0.0005f;
	if (input->TriggerKey(DIK_C)) {
		TENQ->GlitchVerticesLerp(0.08f);
		HoleObject_->GlitchVerticesLerp(1.0f);
		HoleObject2_->GlitchVerticesLerp(5.0f);
		HoleObject3_->GlitchVerticesLerp(5.0f);
	}
	if (input->TriggerKey(DIK_V)) {
		TENQ->StartLerpToOriginalVertices();
		HoleObject_->StartLerpToOriginalVertices();
		HoleObject2_->StartLerpToOriginalVertices();
		HoleObject3_->StartLerpToOriginalVertices();
	}
	camera->CameraDebug();
	if (TENQ) TENQ->EasingDebugUI("TENQ");
	if (HoleObject_) HoleObject_->EasingDebugUI("HoleObject1");
	if (HoleObject2_) HoleObject2_->EasingDebugUI("HoleObject2");
	if (HoleObject3_) HoleObject3_->EasingDebugUI("HoleObject3");
	TENQ->LightDebug("TENQlight");
	HoleObject_->LightDebug("light");
	HoleObject2_->LightDebug("light2");
	HoleObject3_->LightDebug("light3");
	TENQ->ModelDebug("TENQmodel");
	HoleObject_->ModelDebug("model");
	HoleObject2_->ModelDebug("model2");
	HoleObject3_->ModelDebug("model3");
	if (input->TriggerKey(DIK_R)) {
		TENQ->ResetVerticesToOriginal();
		HoleObject_->ResetVerticesToOriginal();
		HoleObject2_->ResetVerticesToOriginal();
		HoleObject3_->ResetVerticesToOriginal();

		this->SetSceneNo(GAMESCENE);
		return;
	}
}
void ClearScene::Draw() {
	// 描画処理
	if (TENQ) {
		TENQ->Draw(textureHandles[TENQ_TEXTURE], camera.get());
	}
	if (HoleObject_) {
		HoleObject_->Draw(textureHandles[NORMAL_HOLE], camera.get());
	}

}
void ClearScene::PostDraw() {
	// ポストエフェクト描画
	postProcess_->Draw();

}
void ClearScene::Release() {
	// リソース解放
	postProcess_.reset();
	postProcess_ = nullptr;
	camera.reset();
	camera = nullptr;
	TENQ.reset();
	TENQ = nullptr;
	HoleObject_.reset();
	HoleObject_ = nullptr;
	HoleObject2_.reset();
	HoleObject2_ = nullptr;
	HoleObject3_.reset();
	HoleObject3_ = nullptr;

}
int ClearScene::GameClose() {
	return false;
}

///Init///
// テクスチャのロード
void ClearScene::LoadTextures() {
	//textureHandles[WHITE] = TextureManager::StoreTexture("Resources/white.png");
	textureHandles[NORMAL_HOLE] = TextureManager::StoreTexture("Resources/10days/white.png");
	textureHandles[TENQ_TEXTURE] = TextureManager::StoreTexture("Resources/10days/world.png");

}

// モデルのロード
void ClearScene::LoadModels()
{
	//ModelManager::GetInstance()->LoadModel("Resources/game", "world.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "Demohole1.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "Demohole2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "Demohole.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "world.obj");
	ModelManager::GetInstance()->LoadModel("Resources/10days/", "start.obj");

}

// オーディオのロード
void ClearScene::LoadAudio()
{
	//audioHandle[BGM] = Audio::SoundLoadWave("Resources/game/Audio/BGM.wav");
}

// 初期化データのセットアップ
void ClearScene::InitializeData() {
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
void ClearScene::HandleGamePadInput() {
	//XINPUT_STATE joyState;
	//if (Input::GetInstance()->GetJoystickState(joyState)) {
	//	HandleStartButton(joyState);
	//	if (isMenu) HandleMenuNavigation(joyState);
	//	else UpdateCameraFOV(joyState);
	//}
}

///Draw///
// オブジェクトの描画
void ClearScene::DrawObjects()
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

void ClearScene::Remake() {

}