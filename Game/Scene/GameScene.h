#pragma once

#include "IScene.h"
#include "Triangle.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Xinput.h"
#include "Particle.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "PostProcess.h"
#include "Fade.h"
#include "DirectXMath.h"
#include "Timer.h"
#include "Collider.h"
#include "Menu.h"
#include "random"
#include <vector>
#include <string>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene {
public:// Public methods
    // コンストラクタ
    GameScene();
    // デストラクタ
    ~GameScene() override;
 
#pragma region Override
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
    // ゲーム終了判定関
    int GameClose() override;
#pragma endregion

public:// Private methods
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

#pragma region Update
    /// <summary>
    /// ゲームパッド入力処理
    /// </summary>
    void HandleGamePadInput();
    /// <summary>
    /// 解放処理
    /// </summary>
    void Remake();
#pragma endregion

#pragma region Draw
    /// <summary>
    /// 全てのオブジェクト
    /// </summary>
    void DrawObjects();
#pragma endregion

private:// Private variable

};
