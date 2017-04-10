//--------------------------------------------------------------------------------------
// File: GamePlay.h
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#pragma once

#include"..\..\StepTimer.h"
#include"..\..\Game.h"
#include "GameScene.h"
#include "..\8\Panel.h"

const wchar_t PanelName[7]
= {
	L'../../Resources/Road1.png',
	L'../../Resources/Road2.png',
	L'../../Resources/Road3.png',
	L'../../Resources/Road4.png',
	L'../../Resources/Road5.png',
	L'../../Resources/Road6.png',
	L'../../Resources/Road7.png',
};

class GamePlay : public GameScene
{
private:
	Panel* m_panel[3][7];

	std::unique_ptr<DirectX::Mouse> m_mouse;
	Microsoft::WRL::ComPtr<ID3D11Device>  m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	DX::StepTimer                                   m_timer;

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