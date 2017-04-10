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
<<<<<<< HEAD

=======
	m_mouse->Update();
	if (m_mouse->IsClickedLeft())
	{
		m_next = PLAY;
	}
>>>>>>> 904c7b17d5e2b6a14be1d5a084a2559150156735
}

void GameTitle::Render()
{
}
