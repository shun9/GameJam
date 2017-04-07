//************************************************/
//* @file  :Pamel.h
//* @brief :道パネルのヘッダー
//* @date  :2017/04/07
//* @author:S.Katou
//************************************************/
#pragma once
#include <SpriteBatch.h>
#include <wrl.h>
#include "WICTextureLoader.h"

class Panel
{
	/*--静的変数--*/
private:
	//１辺の大きさ
	static int SIZE;



	/*--変数--*/
private:
	//画像データ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//スプライト描画に必須
	std::unique_ptr<DirectX::SpriteBatch> m_sprite;


	//上下左右に接しているパネルのポインタ
	//Panel*

	//自身が上下左右に道があるかどうか
	//bool


	/*--関数--*/
public:
	Panel(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext
	, const wchar_t* path);

	~Panel() {
		m_sprite.reset();


		m_texture.Reset();
	}

	//描画
	void Draw(float x, float y);

	//指定方向に接しているパネルを登録
	void Register(Panel* panel);


	//指定方向が進めるかどうかを返す
	bool CanPass();
};