//--------------------------------------------------------------------------------------
// File: GamePlay.h
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#pragma once

#include "..\..\StepTimer.h"
#include "GameScene.h"

class GameTitle : public GameScene
{
private:
public:
	GameTitle();
	~GameTitle();

	void Update();
	void Render();
};