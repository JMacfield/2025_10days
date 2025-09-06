#pragma once
#include "IScene.h"
#include <memory>

// クラスの前方宣言
class Input;
class Camera;
class PostProcess;

class OverScene : public IScene {
public:
    OverScene();
    ~OverScene() override;

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
    std::unique_ptr<Camera> camera_ = nullptr;
    std::unique_ptr<PostProcess> postProcess_ = nullptr;
};