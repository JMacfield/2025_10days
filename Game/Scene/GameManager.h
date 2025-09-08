#pragma once
#include <memory>
#include "IScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "StageSelectScene.h"
#include "OverScene.h"
#include "TitleScene.h"

#include "Input.h"
//#include "Input.h"
class GameManager
{
private:
	// シーンを保持するメンバ変数
	std::array<IScene*, 5> sceneArr_;

	// どのステージを呼び出すかを管理する変数
	int currentSceneNo_ = 0;
	int prevSceneNo_ = 0;
public:
	const char* currentStatus = "Outline Blue";
	GameManager();
	~GameManager();

	int Run();
};