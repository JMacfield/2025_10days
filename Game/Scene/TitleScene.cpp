#include "TitleScene.h"
#include "Input.h"
#include "IScene.h"
#include "Camera.h"
#include "PostProcess.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include <dinput.h>

TitleScene::TitleScene() {}
TitleScene::~TitleScene() {}

void TitleScene::Init() {
    sceneNo = TITLESCENE;



    LoadTextures();
    LoadModels();
    LoadAudio();
    InitializeData();

   
}

void TitleScene::Update() {
    // 何かキーが押されたらステージ選択シーンに移動します
    if (input_->TriggerKey(DIK_SPACE) || input_->TriggerKey(DIK_RETURN)) {
        IScene::SetSceneNo(CLEARSCENE);
    }

    camera_->Update();

    titleSprite_->Update();
    showStart_->Update();
}

void TitleScene::Draw() {
    // ここにタイトルロゴなどの描画処理を記述します
    titleSprite_->Draw(titleSpriteHandle_, { 1.0f,1.0f,1.0f,1.0f });
    showStart_->Draw(showStartHandle_, { 1.0f,1.0f,1.0f,1.0f });
}

void TitleScene::PostDraw() {
    // ポストエフェクトの描画処理
    postProcess_->Draw();
}

void TitleScene::Release() {
    // newしたインスタンスなどの解放処理
    camera_.reset();
    camera_ = nullptr;
}

int TitleScene::GameClose() {
    return false;
}

// --- Private Methods ---

void TitleScene::LoadTextures() {
    // テクスチャの読み込み処理
   
}

void TitleScene::LoadModels() {
    // モデルの読み込み処理
    ModelManager::GetInstance()->LoadModel("Resources/box/", "box.obj");
}

void TitleScene::LoadAudio() {
    // 音源の読み込み処理
}

void TitleScene::InitializeData() {
    input_ = Input::GetInstance();

    // カメラやポストプロセスの初期化
    camera_ = std::make_unique<Camera>();
    camera_->Initialize();

    postProcess_ = std::make_unique <PostProcess>();
    postProcess_->Init();
    postProcess_->SetCamera(camera_.get());

    titleSpriteHandle_ = TextureManager::GetInstance()->StoreTexture("Resources/title/title.png");
    titleSprite_ = std::make_unique<Sprite>();
    titleSprite_->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/title/title.png");
    titleSprite_->SetTextureSize({ 1280.0f,720.0f });

    showStartHandle_ = TextureManager::GetInstance()->StoreTexture("Resources/title/showstart.png");
    showStart_ = std::make_unique<Sprite>();
    showStart_->Init({ 390.0f,300.0f }, { 500.0f,350.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/title/showstart.png");
    showStart_->SetTextureSize({ 300.0f,150.0f });
}