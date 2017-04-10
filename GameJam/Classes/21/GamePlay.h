//--------------------------------------------------------------------------------------
// File: GamePlay.h
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#pragma once
#include <vector>
#include "GameScene.h"
#include "..\8\Panel.h"
<<<<<<< HEAD
#include "../8/MouseManager.h"

struct OPTION
{
	Panel* panel;
	float posX;
	float posY;
};
=======
>>>>>>> 904c7b17d5e2b6a14be1d5a084a2559150156735

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

	//スコア
	int m_score;

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

	//ゲームシステム関連
	void FitOption();
	void PanelSlide();
	void CheckGame();
	void GameOver();
	
	//選んだ選択肢を取得する
	int ChoosedOption();

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