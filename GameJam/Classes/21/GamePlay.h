#pragma once

#include"..\..\StepTimer.h"
#include "..\8\Panel.h"

class GamePlay
{
private:
	Panel m_panel;
//	Player m_player;

public:
	GamePlay();
	~GamePlay();

	void Update();
	void Render();
};