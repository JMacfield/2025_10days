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
    time++;
    TENQ->worldTransform_.rotation_.y += 0.0005f;	// TENQ回転
    // 何かキーが押されたらステージ選択シーンに移動します
    if (input_->TriggerKey(DIK_SPACE) || input_->TriggerKey(DIK_RETURN)) {
        IScene::SetSceneNo(GAMESCENE);
    }
    // イベント処理
    for (const auto& e : events) {
        if (time == e.triggerTime) {
            e.action();
        }
    }
    TENQ->Update();
    TENQ->ModelDebug("TENQ");
    title_->Update();
	title_->ModelDebug("title");
    camera_->Update();

    titleSprite_->Update();
    showStart_->Update();

    if (!Input::GetInstance()->GetJoystickState(joyState)) {
        return;
    }

    if (input_->GetInstance()->TriggerButton(XINPUT_GAMEPAD_A)) {
        IScene::SetSceneNo(GAMESCENE);
    }
}

void TitleScene::Draw() {
    TENQ->Draw(camera_.get());
    title_->Draw(camera_.get());

    // ここにタイトルロゴなどの描画処理を記述します
    //titleSprite_->Draw(titleSpriteHandle_, { 1.0f,1.0f,1.0f,1.0f });
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
    textureHandles[TITLE_TEXTURE] = TextureManager::StoreTexture("Resources/10days/title.png");
    textureHandles[TITLE_TEXTURE2] = TextureManager::StoreTexture("Resources/10days/title2.png");
    textureHandles[TITLE_TEXTURE3] = TextureManager::StoreTexture("Resources/10days/title3.png");
    textureHandles[TITLE_TEXTURE4] = TextureManager::StoreTexture("Resources/10days/title4.png");
    textureHandles[TITLE_TEXTURE5] = TextureManager::StoreTexture("Resources/10days/title5.png");
    textureHandles[TITLE_TEXTURE6] = TextureManager::StoreTexture("Resources/10days/title6.png");
    textureHandles[TENQ_TEXTURE] = TextureManager::StoreTexture("Resources/10days/world.png");
}

void TitleScene::LoadModels() {
    // モデルの読み込み処理
    ModelManager::GetInstance()->LoadModel("Resources/box/", "box.obj");
    ModelManager::GetInstance()->LoadModel("Resources/game", "world.obj");
    ModelManager::GetInstance()->LoadModel("Resources/10days/", "title.obj");
}

void TitleScene::LoadAudio() {
    // 音源の読み込み処理
}

void TitleScene::InitializeData() {
    input_ = Input::GetInstance();


    title_ = std::make_unique<Object3d>();
	title_->Init();
    title_->SetTransform({ { 2.0f,2.0f,2.0f },{ 0.0f,3.14159f,0.0f },{ 0.0f,0.0f,-70.0f } });
    title_->SetModel("title.obj");
    title_->SetTexture(textureHandles[TITLE_TEXTURE]);

	TENQ = std::make_unique<Object3d>();
	TENQ->Init();
	TENQ->SetTransform({ { -300.0f,300.0f,300.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,-170.0f,-70.0f } });
	TENQ->SetModel("world.obj");
	TENQ->SetTexture(textureHandles[TENQ_TEXTURE]);
	TENQ->SetLerpSpeed(0.005f);
    title_->SetLerpSpeed(0.005f);

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
    showStart_->Init({ 390.0f,300.0f }, { 500.0f,380.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/title/showstart.png");
    showStart_->SetTextureSize({ 300.0f,150.0f });
}