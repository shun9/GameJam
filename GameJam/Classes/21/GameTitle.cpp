//--------------------------------------------------------------------------------------
// File: GamePlay.cpp
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#include "../../pch.h"
#include "GameTitle.h"
#include "..\8\ADX2Le.h"
#include "..\..\Sounds\GamePlaySounds.h"

using namespace DirectX;
using namespace SimpleMath;

GameTitle::GameTitle(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
{
	//BGM再生
	ADX2Le::LoadAcb("Sounds\\GamePlaySounds.acb", "Sounds\\GamePlaySounds.awb");
	ADX2Le::Play(CRI_GAMEPLAYSOUNDS__CUE_ID_5);

	m_next = TITLE;

	//マウスの取得
	m_mouse = MouseManager::GetInstance();

	m_spriteBatch = std::make_unique<SpriteBatch>(deviceContext.Get());
	CreateWICTextureFromFile(device.Get(), L"Resources\\title.png", nullptr, m_background.ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(device.Get(), L"Resources\\Click to Start.png", nullptr, m_button.ReleaseAndGetAddressOf());

	//座標設定
	m_backorigin.x = float(0);
	m_backorigin.y = float(0);
	m_buttonorigin.x = float(0);
	m_buttonorigin.y = float(0);

	//矩形の設定
	m_fullscreenRect.left = 0;
	m_fullscreenRect.top = 0;
	m_fullscreenRect.right = 800;
	m_fullscreenRect.bottom = 600;
}

GameTitle::~GameTitle()
{
	m_background.Reset();
	m_spriteBatch.reset();
}

void GameTitle::Update()
{
	//シーン移動
	m_mouse->Update();
	if (m_mouse->IsClickedLeft())
	{
		if (m_next != PLAY)
		{
			ADX2Le::Play(CRI_GAMEPLAYSOUNDS__CUE_ID_4);
		}
		m_next = PLAY;
	}
}

void GameTitle::Render()
{

	m_spriteBatch->Begin();

	m_spriteBatch->Draw(m_background.Get(), Vector2(0.0f, 0.0f), nullptr, Colors::White, 0.f, m_backorigin, 1.25f);
	m_spriteBatch->Draw(m_button.Get(), Vector2(m_fullscreenRect.right / 4.0f, m_fullscreenRect.bottom / 1.5f),
		nullptr, Colors::White, 0.f, m_buttonorigin);

	m_spriteBatch->End();
}
