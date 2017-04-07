//--------------------------------------------------------------------------------------
// File: GamePlay.h
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#pragma once

#include"..\..\StepTimer.h"
#include "..\..\StepTimer.h"
#include "GameScene.h"
#include "..\8\Panel.h"

class GamePlay : public GameScene
{
private:
	Panel m_panel;
public:
	GamePlay();
	~GamePlay();

	void Update();
	void Render();

	void StageMove();
	void PaneFit();
	void PanelSlide();
	void CheckGame();
	void GameOver();
};