#pragma once  
#include <memory>  
#include "IScene.h"  
#include "GameScene.h"  
#include "ClearScene.h"  
#include "StageSelectScene.h"


#include "Input.h"  

class GameManager  
{  
private:  
    // シーンを保持するメンバ変数  
    std::unique_ptr<IScene> sceneArr_[3];  

    // どのステージを呼び出すかを管理する変数  
    int currentSceneNo_ = 0;  
    int prevSceneNo_ = 0;  
public:  
    const char* currentStatus = "Outline Blue";  
    GameManager();  
    ~GameManager();  

    int Run();  
};