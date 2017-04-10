//************************************************/
//* @file  :GamePlay.h
//* @brief :プレイシーン
//* @date  :2017/04/10
//* @author:S.Katou
//************************************************/

#pragma once
#include <vector>
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
	float m_scrollSpd;

	//マウス
	MouseManager* m_mouse;
	int m_mousePosX;
	int m_mousePosY;

	//デバイス関連
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	std::unique_ptr<DirectX::CommonStates> m_commonStates;
	std::unique_ptr<DirectX::SpriteBatch> m_sprite;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_number[10];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_back;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_flame;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_result;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backToTitle;

	//スプライト描画
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	DirectX::SimpleMath::Vector2 m_resultPos;
	DirectX::SimpleMath::Vector2 m_resultPos2;

	//スコア
	int m_score;
	int m_scoreDigit[3];

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
	void DivideScore();

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