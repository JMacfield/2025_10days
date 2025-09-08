#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "Camera.h"
#include "Object3d.h"
#include "Model.h"
#include "PostProcess.h"
#include "Input.h"
#include <memory>

class TitleScene : public IScene {
public:
    TitleScene();
    ~TitleScene() override;

#pragma region Override methods
    // 初期化
    void Init() override;
    // 更新
    void Update() override;
    // 描画
    void Draw() override;
    // ポストエフェクトの描画
    void PostDraw() override;
    // 解放処理
    void Release() override;
    // ゲーム終了判定
    int GameClose() override;
#pragma endregion

private: // Private methods
#pragma region Initialization
    /// <summary>
    /// 全てのテクスチャの読み込み
    /// </summary>
    void LoadTextures();
    /// <summary>
    /// 全てのモデルの読み込み
    /// </summary>
    void LoadModels();
    /// <summary>
    /// 全ての音源の読み込み
    /// </summary>
    void LoadAudio();
    /// <summary>
    /// 全てのデータの初期化
    /// </summary>
    void InitializeData();
#pragma endregion

private: // Private variables
    Input* input_ = nullptr;
    int time = 0;
    std::unique_ptr<Camera> camera_ = nullptr;
    std::unique_ptr<PostProcess> postProcess_;
    std::unique_ptr<Object3d> title_ = nullptr;
    std::unique_ptr<Object3d> TENQ = nullptr;
    struct TitleEvent {
        int triggerTime;
        Object3d* target;
        std::function<void()> action;
    };
    std::vector<TitleEvent> events = {
    {200,  title_.get(), [this] { title_->SetEasingType(GetRandomEasingType()); title_->GlitchVerticesLerp(1.3f); }},
    {400,  TENQ.get(),   [this] { TENQ->SetEasingType(GetRandomEasingType()); TENQ->GlitchVerticesLerp(0.3f); }},
    {600,  title_.get(), [this] { title_->SetEasingType(GetRandomEasingType()); title_->SetTexture(textureHandles[TITLE_TEXTURE3]); title_->StartLerpToOriginalVertices(); }},
    {800,  TENQ.get(),   [this] { TENQ->SetEasingType(GetRandomEasingType()); TENQ->StartLerpToOriginalVertices(); }},
    {1000, title_.get(), [this] { title_->SetEasingType(GetRandomEasingType()); title_->GlitchVerticesLerp(1.3f); }},
    {1200, TENQ.get(),   [this] { TENQ->SetEasingType(GetRandomEasingType()); TENQ->GlitchVerticesLerp(0.3f); }},
    {1400, title_.get(), [this] { title_->SetEasingType(GetRandomEasingType()); title_->SetTexture(textureHandles[TITLE_TEXTURE2]); title_->StartLerpToOriginalVertices(); }},
    {1600,  TENQ.get(),   [this] { TENQ->SetEasingType(GetRandomEasingType()); TENQ->StartLerpToOriginalVertices(); }},
    {1800, title_.get(), [this] { title_->SetEasingType(GetRandomEasingType()); title_->GlitchVerticesLerp(1.3f); }},
    {2000, TENQ.get(),   [this] { TENQ->SetEasingType(GetRandomEasingType()); TENQ->GlitchVerticesLerp(0.3f); }},
    {2200, title_.get(), [this] { title_->SetEasingType(GetRandomEasingType()); title_->SetTexture(textureHandles[TITLE_TEXTURE]); title_->StartLerpToOriginalVertices(); }},
    {2400, TENQ.get(),   [this] { TENQ->SetEasingType(GetRandomEasingType()); TENQ->StartLerpToOriginalVertices(); time = 0; }},
    };
    enum ObjectIndex {
        TITLE,
        TENQ_OJECT,
        OJECT_COUNT
    };
    enum TextureID {
        TITLE_TEXTURE,
        TITLE_TEXTURE2,
        TITLE_TEXTURE3,
        TENQ_TEXTURE,
        TEXTURE_COUNT
    };
    std::array<uint32_t, TEXTURE_COUNT> textureHandles;

    // タイトルスプライト
    std::unique_ptr<Sprite> titleSprite_ = nullptr;
    uint32_t titleSpriteHandle_;

    // スタート画像
    std::unique_ptr<Sprite> showStart_ = nullptr;
    uint32_t showStartHandle_;
};