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
#include "Loader.h"
#include <vector>
#include <string>
#include <memory>

#include "../TestPlane/TestWall.h"
#include "../Player/Player.h"
#include "../Camera/FollowCamera.h"
#include "../Collision/CollisionManager.h"
#include "../Obstacles/ObstacleManager.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene {
public:// Public methods
    void ShowDeathUI();

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

    std::vector<std::unique_ptr<Object3d>> objectList_;
    enum ObjectIndex {
        TENQ,
        RULE,
        MAP
    };
    bool isLerping_ = false;
    float lerpT = 0.0f; // 追加: ラープ係数
    std::unique_ptr<PostProcess> postProcess_ = nullptr;
    std::unique_ptr<Camera> camera = nullptr;
    Input* input = nullptr;
    enum TextureID {
        NORMAL_HOLE,
        TENQ_TEXTURE,
		MAP_TEXTURE,
        TEXTURE_COUNT // テクスチャの総数
    };
    std::array<uint32_t, TEXTURE_COUNT> textureHandles;

    // Blenderの配置情報を読み取るローダー
    //Loader* loader_;
   /* std::vector<Object3d*> objects_;
    std::vector<Collider*> colliders_;
    std::vector<std::string> wallTypes_;*/
    std::unique_ptr<ObstacleManager> obstacleManager_;

    // 入力
    Input* input_;

    // 当たり判定管理クラス
    CollisionManager* collisionManager_;

    // 追従カメラ
    std::unique_ptr<FollowCamera> followCamera_;

    // 自機
    std::unique_ptr<Player> player_;

    // 当たり判定確認用の壁
    std::vector<TestWall*> testWall_;

    // 床(仮)
    std::unique_ptr<Object3d> floor_;
    // 床のテクスチャ
    uint32_t floorTex_;
    uint32_t damageWallTex_;
    uint32_t greenWallTex_;

    // 操作UI用
    enum UITexture {
        UI_TEXTURE,
        UI_TEXTURE2,
        UI_TEXTURE3,
        UI_TEXTURE_COUNT
    };

    bool ismapswitch = false;
    std::array<uint32_t, UI_TEXTURE_COUNT> uiTextureHandles_;
    std::array<std::unique_ptr<Sprite>, UI_TEXTURE_COUNT> uiSprite_;

    std::unique_ptr<Sprite> overSprite_ = nullptr;
    uint32_t overSpriteHandle_;

    std::unique_ptr<Sprite> showContinue_ = nullptr;
    uint32_t showContinueHandle_;

    uint32_t jumpSE;
    uint32_t switchSE;
};
