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

}

void GameTitle::Render()
{
}
