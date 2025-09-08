#include "ClearScene.h"
#include "Input.h"
#include "IScene.h"
#include "Camera.h"
#include "PostProcess.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include <dinput.h>

ClearScene::ClearScene() {}
ClearScene::~ClearScene() {}

void ClearScene::Init() {
    sceneNo = CLEARSCENE;



    LoadTextures();
    LoadModels();
    LoadAudio();
    InitializeData();


}

void ClearScene::Update() {
    // 何かキーが押されたらステージ選択シーンに移動します
    if (input_->TriggerKey(DIK_SPACE) || input_->TriggerKey(DIK_RETURN)) {
        IScene::SetSceneNo(OVERSCENE);
    }

    camera_->Update();

    clearSprite_->Update();
}

void ClearScene::Draw() {
    // ここにタイトルロゴなどの描画処理を記述します
    clearSprite_->Draw(clearSpriteHandle_, { 1.0f,1.0f,1.0f,1.0f });
}

void ClearScene::PostDraw() {
    // ポストエフェクトの描画処理
    postProcess_->Draw();
}

void ClearScene::Release() {
    // newしたインスタンスなどの解放処理
    camera_.reset();
    camera_ = nullptr;
}

int ClearScene::GameClose() {
    return false;
}

// --- Private Methods ---

void ClearScene::LoadTextures() {
    // テクスチャの読み込み処理

}

void ClearScene::LoadModels() {
    // モデルの読み込み処理
    ModelManager::GetInstance()->LoadModel("Resources/box/", "box.obj");
}

void ClearScene::LoadAudio() {
    // 音源の読み込み処理
}

void ClearScene::InitializeData() {
    input_ = Input::GetInstance();

    // カメラやポストプロセスの初期化
    camera_ = std::make_unique<Camera>();
    camera_->Initialize();

    postProcess_ = std::make_unique <PostProcess>();
    postProcess_->Init();
    postProcess_->SetCamera(camera_.get());

    clearSpriteHandle_ = TextureManager::GetInstance()->StoreTexture("Resources/gameclear/gameclear.png");
    clearSprite_ = std::make_unique<Sprite>();
    clearSprite_->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/gameclear/gameclear.png");
    clearSprite_->SetTextureSize({ 1280.0f,720.0f });
}