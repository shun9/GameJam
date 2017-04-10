//2017年4月7日　制作
//製作者　山岸正都
//プレイヤークラスのソースファイル

#include "Player.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;


//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @return なし
//----------------------------------------------------------------------
Player::Player(float x, float y)
{
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	m_sprite = std::unique_ptr<DirectX::SpriteBatch>(new SpriteBatch(deviceContext.Get()));

	m_pos.x = x;
	m_pos.y = y;

	m_panel = nullptr;
	m_state = false;
	m_direction = 5;
	m_work_num = 0;

	CreateWICTextureFromFile(device.Get(), L"Resources\\player.png", nullptr, m_texture[0].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(device.Get(), L"Resources\\player1.png", nullptr, m_texture[1].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(device.Get(), L"Resources\\player2.png", nullptr, m_texture[2].ReleaseAndGetAddressOf());

	m_texture_num = 0;
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
Player::~Player()
{
	m_sprite.reset();
	for (int i = 0; i < 3; i++)
	{
		m_texture[i].Reset();
	}
}

//----------------------------------------------------------------------
//! @brief プレイヤーの情報の更新
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::Update()
{
	work();
}

//----------------------------------------------------------------------
//! @brief プレイヤーの描画
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::Render()
{
	m_sprite->Begin();

	m_sprite->Draw(m_texture[m_texture_num].Get(), XMFLOAT2(m_pos.x, m_pos.y), nullptr, Colors::White);

	m_sprite->End();
}

//----------------------------------------------------------------------
//! @brief プレイヤーが歩けるかどうか返す関数
//!
//! @param[in] なし
//!
//! @return プレイヤーがどの方向に動けるか
//----------------------------------------------------------------------
int Player::isWork(int direction)
{
	//パネルの情報が入っていなければその状態を返す
	if (m_panel == nullptr)
	{
		return 5;
	}
	else if (m_panel->CanPass(direction))
	{
		return direction;
	}
	else
	{
		return 5;
	}
}

//----------------------------------------------------------------------
//! @brief プレイヤーを移動させる関数
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::work()
{
	//プレイヤーが動いていない場合
	if (m_state == false)
	{
		//動けるかどうかを確認
		for (int direction = 0; direction < 6; direction++)
		{
			if (isWork(direction))
			{
				m_state = true;
				m_direction = direction;
			}
			else
			{
				m_state = false;
			}
		}
	}

	//動ける場合
	if (m_state == true)
	{
		//プレイヤーが動ける方向に移動させ、カウントを進める
		if (m_direction == TOP)
		{
			m_pos.y -= 2;
			m_work_num++;
		}
		else if (m_direction == BOTTOM)
		{
			m_pos.y += 2;
			m_work_num++;
		}
		else if (m_direction == LEFT)
		{
			m_pos.x -= 2;
			m_work_num++;
		}
		else if (m_direction == BOTTOM)
		{
			m_pos.x += 2;
			m_work_num++;
		}
	}

	//１パネル分進んだら
	if (m_work_num == 64)
	{
		//プレイヤーを歩かせない状態にして、カウントを０にする
		m_state = false;
		m_work_num = 0;
	}

}

//----------------------------------------------------------------------
//! @brief プレイヤーの位置にあるパネルを登録する関数
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::registerPanel(Panel* panel)
{
	m_panel = panel;
}

//----------------------------------------------------------------------
//! @brief プレイヤーの画像を変更する関数
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::changeTexture()
{
	//動いていない場合
	if (m_state == false)
	{
		m_texture_num = 0;
	}
	//動ける場合
	else if (m_state == true)
	{
		//動いている歩数で画像を差し替える
		for (int i = 0; i < 64; i++)
		{
			if (m_work_num == 1 || m_work_num == 16 || m_work_num == 32 || m_work_num == 48)
			{
				m_texture_num = 1;
			}
			else if (m_work_num == 8 || m_work_num == 24 || m_work_num == 40 || m_work_num == 56)
			{
				m_texture_num = 2;
			}
		}
	}
}

//----------------------------------------------------------------------
//! @brief プレイヤーを強制的に移動させる関数
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::compelMove()
{
	//動いていない状態なら
	if (m_state == false)
	{
		return;
	}
	//歩いた距離がパネル半個分より小さいなら移動させる
	else if (128 - m_work_num * 2 > 64)
	{
		if (m_direction == TOP)
		{

		}
		else if (m_direction == BOTTOM)
		{

		}
		else if (m_direction == RIGHT)
		{

		}
		else if (m_direction == LEFT)
		{

		}
		else
		{
			return;
		}
	}

}