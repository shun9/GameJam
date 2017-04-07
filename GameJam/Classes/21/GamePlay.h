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

<<<<<<< HEAD
class Player;
class Panel;

class GamePlay : public Game
{
private:
	Panel* m_panel;
	Player* m_player;

=======
class GamePlay : public GameScene
{
private:
	Panel m_panel;
>>>>>>> GamePlay
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