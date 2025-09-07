#include "TitleScene.h"
#include "Input.h"
#include "IScene.h"
#include "Camera.h"
#include "PostProcess.h"
#include "TextureManager.h"
#include <dinput.h>

TitleScene::TitleScene() {}
TitleScene::~TitleScene() {}

void TitleScene::Init() {
    sceneNo = TITLESCENE;



    LoadTextures();
    LoadModels();
    LoadAudio();
    InitializeData();

    // 床
    floor_ = std::make_unique<Object3d>();
    floor_->Init();
    floor_->SetModel("box.obj");
    floor_->worldTransform_.scale_ = { 10.0f,0.1f,100.0f };
    floor_->worldTransform_.translation_ = { 0.0f,-1.0f,0.0f };
}

void TitleScene::Update() {
    // 何かキーが押されたらステージ選択シーンに移動します
    if (input_->TriggerKey(DIK_SPACE) || input_->TriggerKey(DIK_RETURN)) {
        IScene::SetSceneNo(GAMESCENE);
    }

    camera_->Update();

    floor_->Update();
}

void TitleScene::Draw() {
    // ここにタイトルロゴなどの描画処理を記述します
    floor_->Draw(floorTex_, camera_.get());
}

void TitleScene::PostDraw() {
    // ポストエフェクトの描画処理
    postProcess_->Draw();
}

void TitleScene::Release() {
    // newしたインスタンスなどの解放処理
    camera_.reset();
    camera_ = nullptr;
    delete postProcess_;
    postProcess_ = nullptr;
}

int TitleScene::GameClose() {
    return false;
}

// --- Private Methods ---

void TitleScene::LoadTextures() {
    // テクスチャの読み込み処理
    floorTex_ = TextureManager::StoreTexture("Resources/white.png");
}

void TitleScene::LoadModels() {
    // モデルの読み込み処理
}

void TitleScene::LoadAudio() {
    // 音源の読み込み処理
}

void TitleScene::InitializeData() {
    input_ = Input::GetInstance();

    // カメラやポストプロセスの初期化
    camera_ = std::make_unique<Camera>();
    camera_->Initialize();

    postProcess_ = new PostProcess();
    postProcess_->Init();
    postProcess_->SetCamera(camera_.get());
}