//--------------------------------------------------------------------------------------
// File: GamePlay.h
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#pragma once

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <wrl.h>
#include "WICTextureLoader.h"
#include "..\..\StepTimer.h"
#include"..\..\Game.h"
#include "GameScene.h"
#include "..\8\MouseManager.h"

class GameTitle : public GameScene
{
private:
	MouseManager* m_mouse;
//背景
	//画像データ
	RECT m_fullscreenRect;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_background;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_button;
	std::unique_ptr<DirectX::CommonStates> m_commonStates;

	//スプライト描画
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//座標
	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 m_backorigin;
	DirectX::SimpleMath::Vector2 m_buttonorigin;


	bool m_clickNow;
	bool m_clickBefore;

public:
	GameTitle(Microsoft::WRL::ComPtr<ID3D11Device> device
		, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);
	~GameTitle();

	void Update();
	void Render();
};