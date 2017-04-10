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
}

GameTitle::~GameTitle()
{
}

void GameTitle::Update()
{
	auto state = m_mouse->GetState();
	if (state.leftButton)
	{
		m_next = PLAY;
	}
}

void GameTitle::Render()
{
}
