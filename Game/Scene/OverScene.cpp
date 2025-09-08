#include "OverScene.h"
#include "Input.h"
#include "IScene.h"
#include "Camera.h"
#include "PostProcess.h"
#include <dinput.h>

OverScene::OverScene() {}
OverScene::~OverScene() {}

void OverScene::Init() {
    sceneNo = OVERSCENE;

    LoadTextures();
    LoadModels();
    LoadAudio();
    InitializeData();
}

void OverScene::Update() {
    // 何かキーが押されたらタイトルシーンに戻ります
    if (input_->TriggerKey(DIK_SPACE) || input_->TriggerKey(DIK_RETURN)) {
        IScene::SetSceneNo(TITLESCENE);
    }

    overSprite_->Update();
}

void OverScene::Draw() {
    // 「Game Over」などの描画処理を記述します
    overSprite_->Draw(overSpriteHandle_, { 1.0f,1.0f,1.0f,1.0f });
}

void OverScene::PostDraw() {
    // ポストエフェクトの描画処理
    postProcess_->Draw();
}

void OverScene::Release() {
    // newしたインスタンスなどの解放処理
    camera_.reset();
    postProcess_.reset();
}

int OverScene::GameClose() {
    return false;
}

// --- Private Methods ---

void OverScene::LoadTextures() {
    // テクスチャの読み込み処理
}

void OverScene::LoadModels() {
    // モデルの読み込み処理
}

void OverScene::LoadAudio() {
    // 音源の読み込み処理
}

void OverScene::InitializeData() {
    input_ = Input::GetInstance();

    // カメラやポストプロセスの初期化
    camera_ = std::make_unique<Camera>();
    camera_->Initialize();

    postProcess_ = std::make_unique<PostProcess>();
    postProcess_->Init();

    overSpriteHandle_ = TextureManager::GetInstance()->StoreTexture("Resources/gameover/gameover.png");
    overSprite_ = std::make_unique<Sprite>();
    overSprite_->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/gameover/gameover.png");
    overSprite_->SetTextureSize({ 1280.0f,720.0f });
}