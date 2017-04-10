//--------------------------------------------------------------------------------------
// File: GamePlay.h
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#pragma once

#include "..\..\StepTimer.h"
#include"..\..\Game.h"
#include "GameScene.h"

class GameTitle : public GameScene
{
private:
	std::unique_ptr<DirectX::Mouse> m_mouse;

public:
	GameTitle();
	~GameTitle();

	void Update();
	void Render();
};