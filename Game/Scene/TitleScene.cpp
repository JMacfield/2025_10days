#include "TitleScene.h"
#include "Input.h"
#include "IScene.h"
#include "Camera.h"
#include "PostProcess.h"
#include <dinput.h>

TitleScene::TitleScene() {}
TitleScene::~TitleScene() {}

void TitleScene::Init() {
    LoadTextures();
    LoadModels();
    LoadAudio();
    InitializeData();
}

void TitleScene::Update() {
    // 何かキーが押されたらステージ選択シーンに移動します
    if (input_->TriggerKey(DIK_SPACE) || input_->TriggerKey(DIK_RETURN)) {
        IScene::SetSceneNo(GAMESCENE);
    }
}

void TitleScene::Draw() {
    // ここにタイトルロゴなどの描画処理を記述します
}

void TitleScene::PostDraw() {
    // ポストエフェクトの描画処理
}

void TitleScene::Release() {
    // newしたインスタンスなどの解放処理
    camera_.reset();
    postProcess_.reset();
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
}

void TitleScene::LoadAudio() {
    // 音源の読み込み処理
}

void TitleScene::InitializeData() {
    input_ = Input::GetInstance();

    // カメラやポストプロセスの初期化
    camera_ = std::make_unique<Camera>();
    camera_->Initialize();

    postProcess_ = std::make_unique<PostProcess>();
    postProcess_->Init();
}