#include "GameManager.h"
#include <Windows.h>
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "SRVManager.h"
#include "PSOModel.h"
#include "PSOAnimationModel.h"
#include "PSOSprite.h"
#include "PSOParticle.h"
#include "PSOSkybox.h"
#include "PSOPostEffect.h"
#include "PostProcess.h"
#include "Input.h"
#include "Mesh.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Audio.h"
#include "Camera.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Model.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Object3dCommon.h"
#include "Triangle.h"

#include "VertexData.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "mathFunction.h"


GameManager::GameManager() {
	// 各シーンの登録
	sceneMap_[GAMESCENE] = []() { return std::make_shared<GameScene>(); };
	sceneMap_[CLEARSCENE] = []() { return std::make_shared<ClearScene>(); };
	sceneMap_[STAGESELECTSCENE] = []() { return std::make_shared<StageSelectScene>(); };
}
GameManager::~GameManager() {
	if (sceneArr_) {
		sceneArr_->Release();
		sceneArr_.reset();
	}
}

const char kWindowTitle[] = "LE3A";

int GameManager::Run() {
	//DirectXCommon::D3DResourceLeakChecker leakCheck;

	WinAPI* sWinAPI = WinAPI::GetInstance();
	sWinAPI->Initialize(L"NeonShift");

	DirectXCommon* sDirctX = DirectXCommon::GetInstance();
	sDirctX->Initialize();

	ImGuiCommon* imGuiCommon = ImGuiCommon::GetInstance();
	imGuiCommon->Initialize();

	Audio* sAudio = Audio::GetInstance();
	sAudio->Initialize();

	Object3dCommon* sObjectCommon = Object3dCommon::GetInstance();
	sObjectCommon->Init();

	ModelManager* sModelManager = ModelManager::GetInstance();
	sModelManager->init();

	TextureManager* sTextureManager = TextureManager::GetInstance();
	sTextureManager->Init();

	PSO* pso = PSO::GetInstance();
	pso->CreatePipelineStateObject();

	PSOSprite* psoSprite = PSOSprite::GetInstance();
	psoSprite->CreatePipelineStateObject();

	PSOParticle* psoParticle = PSOParticle::GetInstance();
	psoParticle->CreatePipelineStateObject();

	PSOSkybox* psoSkybox = PSOSkybox::GetInstance();
	psoSkybox->CreatePipelineStateObject();

	PSOPostEffect* pSOPostEffect = PSOPostEffect::GetInstance();
	pSOPostEffect->Init();

	// シーンのチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = IScene::GetSceneNo();
	//post->Init();
	sceneArr_ = sceneMap_[currentSceneNo_]();
	sceneArr_->Init();


	Input* sInput = Input::GetInstance();
	sInput->Initialize();
	// ウィンドウの×ボタンが押されるまでループ
	while (true)  // ゲームループ
	{
		// Windowsのメッセージ処理
		if (sWinAPI->ProcessMessage()) {
			// ゲームループを抜ける
			break;
		}
		// ゲームの処理の開始
		sDirctX->tempRender();
		// ImGui
		ImGuiCommon::GetInstance()->UICreate();
		//ImGuiの更新
		ImGuiCommon::GetInstance()->Update();

		sInput->Update();

		// 追跡するための変数
#ifdef _DEBUG

		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.7f, 0.0f, 0.2f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.3f, 0.0f, 0.1f, 0.5f));
		ImGui::Begin("PostEffect", nullptr, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("PostEffect")) {
				if (ImGui::MenuItem("Normal")) {
					IPostEffectState::SetEffectNo(kFullScreen);
					currentStatus = "Normal effect applied";
				}
				if (ImGui::MenuItem("Outline")) {
					IPostEffectState::SetEffectNo(kLuminanceBasedOutline);
					currentStatus = "Outline effect applied";
				}
				if (ImGui::MenuItem("Grayscale")) {
					IPostEffectState::SetEffectNo(kGrayscale);
					currentStatus = "Grayscale effect applied";
				}
				if (ImGui::MenuItem("Fog")) {
					IPostEffectState::SetEffectNo(kFog);
					currentStatus = "Fog effect applied";
				}
				if (ImGui::MenuItem("Dissololve")) {
					IPostEffectState::SetEffectNo(kDissolve);
					currentStatus = "Dissolve effect applied";
				}
				if (ImGui::MenuItem("Gaussian")) {
					IPostEffectState::SetEffectNo(kGaussianFilter);
					currentStatus = "Gaussian blur effect applied";
				}
				if (ImGui::MenuItem("Vignett")) {
					IPostEffectState::SetEffectNo(kVignetting);
					currentStatus = "Vignette effect applied";
				}
				if (ImGui::MenuItem("Random")) {
					IPostEffectState::SetEffectNo(kRandom);
					currentStatus = "Random effect applied";
				}
				if (ImGui::MenuItem("Radialblur")) {
					IPostEffectState::SetEffectNo(kRadialBlur);
					currentStatus = "Radial blur effect applied";
				}
				if (ImGui::MenuItem("Outline Black")) {
					IPostEffectState::SetEffectNo(kOutlineBlack);
					currentStatus = "Outline Black effect applied";
				}
				if (ImGui::MenuItem("Outline Blue")) {
					IPostEffectState::SetEffectNo(kOutlineBlue);
					currentStatus = "Outline Blue effect applied";
				}
				if (ImGui::MenuItem("Outline purple")) {
					IPostEffectState::SetEffectNo(kOutlinePurple);
					currentStatus = "Outline Test effect applied";
				}
				if (ImGui::MenuItem("TV")) {
					IPostEffectState::SetEffectNo(kTVEffect);
					currentStatus = "TV effect applied";
				}
				if (ImGui::MenuItem("chromatic aberration")) {
					IPostEffectState::SetEffectNo(kChromaticAberration);
					currentStatus = "chromatic aberration effect applied";
				}
				if (ImGui::MenuItem("bloom")) {
					IPostEffectState::SetEffectNo(kBloom);
					currentStatus = "bloom effect applied";
				}
				if (ImGui::MenuItem("HSV")) {
					IPostEffectState::SetEffectNo(kHSVFilter);
					currentStatus = "HSV effect applied";
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::Text("Status: %s", currentStatus);
		ImGui::Text("Scene : %d", currentSceneNo_);
		ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
#endif

		// シーンのチェック
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = IScene::GetSceneNo();

		// シーン変更チェック
		if (prevSceneNo_ != currentSceneNo_) {
			sceneArr_ = sceneMap_[currentSceneNo_]();
			sceneArr_->Init();
		}

		///
		/// ↓更新処理ここから
		///
		sceneArr_->Update(); // シーンごとの更新処理

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		sceneArr_->Draw();


		///
		/// ↑描画処理ここまで
		///
		sDirctX->BeginFrame();
		sDirctX->ChangeDepthStatetoRead();
		sceneArr_->PostDraw();
		// フレームの終了
		//スワップチェーン
		sDirctX->ChangeDepthStatetoRender();
		sDirctX->ViewChange();
		sAudio->GetIXAudio().Reset();
		// ESCキーが押されたらループを抜ける
		if (sceneArr_->GameClose()) {
			sceneArr_->Release();
			break;
		}
	}

	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");


	/*------------------------------------------------------------

	-------------------------------------------------------------*/

	sceneArr_[currentSceneNo_]->Release();
	sModelManager->Finalize();
	TextureManager::GetInstance()->Release();
	SRVManager::GetInstance()->Release();
	sDirctX->Release();
	sWinAPI->Finalize();

	return 0;
}