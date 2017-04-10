//--------------------------------------------------------------------------------------
// File: GamePlay.cpp
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#include "..\..\pch.h"
#include "..\..\Game.h"
#include "GamePlay.h"

GamePlay::GamePlay()
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_panel[j][i] = nullptr;
			if (!(i == 0 && j == 1))
			{
				m_panel[j][i] = new Panel(m_d3dDevice, m_d3dContext,
					&PanelName[0], Panel::passRightLeft);
			}
			else
			{
				SetPanel(i, j);
			}
			m_panel[j][i]->Draw();
		}
	}
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
}

GamePlay::~GamePlay()
{
}

void GamePlay::Update()
{
	auto state = m_mouse->GetState();

	CheckGame();
	StageMove();

	if (state.leftButton)
	{
	}
}

void GamePlay::Render()
{

}

void GamePlay::SetPanel(int x, int y)
{
	int type;
	type = rand() % 7;
	m_panel[y][x] = nullptr;
	m_panel[y][x] = new Panel(m_d3dDevice, m_d3dContext, &PanelName[type], SetType(type));
}

PanelPass GamePlay::SetType(int type)
{
	switch (type)
	{
	case 0:return Panel::passRightLeft; break;
	case 1:return Panel::passTopBottom; break;
	case 2:return Panel::passTopRight; break;
	case 3:return Panel::passTopLeft; break;
	case 4:return Panel::passBottomRight; break;
	case 5:return Panel::passBottomLeft; break;
	default: return Panel::passNone;
	}
}

void GamePlay::StageMove()
{
}

void GamePlay::PaneFit()
{
}

void GamePlay::PanelSlide()
{
}

void GamePlay::CheckGame()
{
}

void GamePlay::GameOver()
{
}
