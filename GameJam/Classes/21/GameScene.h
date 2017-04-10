//--------------------------------------------------------------------------------------
// File: GamePlay.h
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#pragma once

#include"..\..\StepTimer.h"
#include"..\..\Game.h"

class GameScene
{
public:
	virtual ~GameScene() {};

	int m_next;
	//èÉêàâºëzä÷êî
	virtual void Update() = 0;
	virtual void Render() = 0;

};