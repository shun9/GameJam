//--------------------------------------------------------------------------------------
// File: GamePlay.cpp
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#include "..\..\pch.h"
#include "GameTitle.h"

GameTitle::GameTitle()
{
	m_next = TITLE;
	m_mouse = MouseManager::GetInstance();

}

GameTitle::~GameTitle()
{
}

void GameTitle::Update()
{
	m_mouse->Update();
	if (m_mouse->IsClickedLeft())
	{
		m_next = PLAY;
	}
}

void GameTitle::Render()
{
}