//2017年4月7日　制作
//製作者　山岸正都
//プレイヤークラスのヘッダーファイル

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include <wrl.h>
#include <..\..\Users\s162188\Documents\GameJam\GameJam\Classes\8\Panel.h>

class Player
{
private:
	std::unique_ptr<DirectX::SpriteBatch> m_sprite;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture[3];
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_pos;		//現在の位置
	int m_direction;					//プレイヤーの動く方向
	int m_work_num;						//プレイヤーが動いた距離
	int m_texture_num;					//現在の画像
	bool m_state;						//プレイヤーが歩けるかの状態

	Panel* m_panel;						//プレイヤーが乗っているパネルの情報
	int isWork(int direction);			//プレイヤーがどの方向に動けるかを返す関数
	void work();						//プレイヤーを動かす関数
	void changeTexture();				//プレイヤーの画像を変更する関数

public:
	Player(float x, float y);
	~Player();
	void Update();
	void Render();
	void registerPanel(Panel* panel);	//プレイヤーの位置にあるパネルを登録する関数
};
