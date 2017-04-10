//--------------------------------------------------------------------------------------
// File: GamePlay.h
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#pragma once

#include "..\..\StepTimer.h"
#include"..\..\Game.h"
#include "GameScene.h"
#include "..\8\MouseManager.h"

class GameTitle : public GameScene
{
private:
	MouseManager* m_mouse;

public:
	GameTitle();
	~GameTitle();


	void Update();
	void Render();
};