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
#include <functional>

class ClearScene : public IScene {
public:
    ClearScene();
    ~ClearScene() override;
#pragma region Override methods
    // ������
    void Init() override;
    // �X�V
    void Update() override;
    // �`��
    void Draw() override;
    // �|�X�g�G�t�F�N�g�̕`��
    void PostDraw() override;
    // �������
    void Release() override;
    // �Q�[���I�������
    int GameClose() override;
#pragma endregion

private:// Private methods
#pragma region Initialization
    /// <summary>
    /// �S�Ẵe�N�X�`���̓ǂݍ���
    /// </summary>
    void LoadTextures();
    /// <summary>
    /// �S�Ẵ��f���̓ǂݍ���
    /// </summary>
    void LoadModels();
    /// <summary>
    /// �S�Ẳ����̓ǂݍ���
    /// </summary>
    void LoadAudio();
    /// <summary>
    /// �S�Ẵf�[�^�̏�����
    /// </summary>
    void InitializeData();
#pragma endregion

#pragma region Update
    /// <summary>
    /// �Q�[���p�b�h���͏���
    /// </summary>
    void HandleGamePadInput();
    /// <summary>
    /// �������
    /// </summary>
    void Remake();
#pragma endregion

#pragma region Draw
    /// <summary>
    /// �S�ẴI�u�W�F�N�g
    /// </summary>
    void DrawObjects();
#pragma endregion

private:// Private variable
    bool isLerping_ = false;
    float lerpT = 0.0f; // �ǉ�: ���[�v�W��
    std::unique_ptr<PostProcess> postProcess_ = nullptr;
    std::unique_ptr<Camera> camera = nullptr;
    Input* input = nullptr;
    std::unique_ptr<Object3d> TENQ = nullptr;
    std::unique_ptr<Object3d> HoleObject_ = nullptr;
    std::unique_ptr<Object3d> HoleObject2_ = nullptr;
    std::unique_ptr<Object3d> HoleObject3_ = nullptr;
    enum TextureID {
        NORMAL_HOLE,
        TENQ_TEXTURE,
        TEXTURE_COUNT // �e�N�X�`���̑���
    };
    std::array<uint32_t, TEXTURE_COUNT> textureHandles;
};