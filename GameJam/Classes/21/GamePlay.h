//--------------------------------------------------------------------------------------
// File: GamePlay.h
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#pragma once
#include <vector>
#include <SpriteBatch.h>
#include <wrl.h>
#include "WICTextureLoader.h"
#include "GameScene.h"
#include "..\8\Panel.h"

#include "../8/MouseManager.h"

#include "../20/Player.h"

struct OPTION
{
	Panel* panel;
	float posX;
	float posY;
};

class GamePlay : public GameScene
{
	/*--静的変数--*/
public:
	static const int MAP_Y;
	static const int MAP_X;
	static const int MAP_POS_X;
	static const int MAP_POS_Y;

private:
	static const int MAX_OPTION;
	static const float OPTION_POS_X[3];
	static const float OPTION_POS_Y;


	/*--メンバ変数--*/
private:
	//プレイヤー
	Player* m_player;

	//ステージのパネル
	Panel*** m_panel;
	
	//選択肢のパネル
	OPTION* m_option;

	//選択中の番号
	int m_numChoosed;

	//カウント
	int m_cntTime;
	float m_scrollPos;

	//マウス
	MouseManager* m_mouse;
	int m_mousePosX;
	int m_mousePosY;

	//デバイス関連
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_result;

	//スプライト描画
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//スコア
	int m_score;

	//ゲームオーバーフラグ
	bool m_isGameOver;

	/*--メンバ関数--*/
public:
	GamePlay(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	~GamePlay();

	void Update();
	void Render();

private:
	//更新処理関連
	void UpdateStage();
	void UpdateOption();
	void UpdatePlayer();

	//ゲームシステム関連
	void FitOption();
	void PanelSlide();
	bool IsDead();
	void GameOver();
	
	//選んだ選択肢を取得する
	int ChoosedOption();

	//パネルを繋げる
	void LinkPanel();

	//描画関連
	void DrawStage();
	void DrawOption();

	//作成関連
	void CreateStage();
	void CreateOption();

	//削除関連
	void DeleteStage();
	void DeleteOption();
};