//************************************************/
//* @file  :Pamel.h
//* @brief :道パネルのヘッダー
//* @date  :2017/04/09
//* @author:S.Katou
//************************************************/
#pragma once
#include <SpriteBatch.h>
#include <wrl.h>
#include "WICTextureLoader.h"

//上下左右
enum Direction
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
};


//パネルの通過可能か示す構造体
struct PanelPass
{
	bool top;
	bool bottom;
	bool left;
	bool right;
	wchar_t* path;
};

//パネルクラス
class Panel
{
	/*--静的変数--*/
public:
	//１辺の大きさ
	static const int SIZE;
	static const int MAX_PASS_NUM;

	//パスの一覧
	static const PanelPass passTopRight;
	static const PanelPass passTopLeft;
	static const PanelPass passBottomRight;
	static const PanelPass passBottomLeft;
	static const PanelPass passRightLeft;
	static const PanelPass passTopBottom;
	static const PanelPass passNone;

	/*--静的関数--*/
public:
	//ランダムなパネルのパスを返す
	static PanelPass GetRandomPass();


	/*--変数--*/
private:
	//画像データ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//スプライト描画に必須
	std::unique_ptr<DirectX::SpriteBatch> m_sprite;


	//上下左右に接しているパネルのポインタ
	Panel* m_linkPanel[4];

	//自身が上下左右に道があるかどうか
	bool m_canPass[4];


	/*--関数--*/
public:
	//コンストラクタ
	Panel(Microsoft::WRL::ComPtr<ID3D11Device> device
		, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext
		, const PanelPass& pass);


	//デストラクタ
	~Panel();

	//描画
	void Draw(float x, float y);

	//指定方向に接しているパネルを登録
	void Register(Panel* panel,int direction);


	//指定方向が進めるかどうかを返す
	bool CanPass(int direction);


private:
	//指定方向のパネルに進めるかどうかを返す
	bool CanPassLinkPanel(int direction);
};