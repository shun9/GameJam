//2017年4月7日　制作
//製作者　山岸正都
//プレイヤークラスのヘッダーファイル

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include <wrl.h>
#include "../8/Panel.h"
#include <CommonStates.h>

class Player
{
private:
	std::unique_ptr<DirectX::SpriteBatch> m_sprite;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture[3];
	DirectX::SimpleMath::Vector2 m_pos;					//現在の位置
	std::unique_ptr<DirectX::CommonStates> m_commonStates;
	int m_direction;									//プレイヤーの動く方向
	int m_work_num;										//プレイヤーが動いた距離
	int m_texture_num;									//現在の画像
	bool m_state;										//プレイヤーが歩けるかの状態

	Panel* m_panel;										//プレイヤーが乗っているパネルの情報
	bool isWork(int direction);							//プレイヤーがどの方向に動けるかを返す関数
	void work();										//プレイヤーを動かす関数
	void changeTexture();								//プレイヤーの画像を変更する関数
	void compelMove();									//プレイヤーを強制的に移動させる関数
	bool isDirection(int direction, int old_direction);	//前回の方向と今から動く方向を比べる関数

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;

public:
	Player(float x, float y, 
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	~Player();
	void Update(float scroll);
	void Render();
	void registerPanel(Panel* panel);	//プレイヤーの位置にあるパネルを登録する関数

	DirectX::SimpleMath::Vector2 getPos() { return m_pos; }

};
