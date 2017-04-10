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
Player::Player(float x, float y	,Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	:m_device(device)
	,m_deviceContext(context)
{
	m_sprite = std::unique_ptr<DirectX::SpriteBatch>(new SpriteBatch(m_deviceContext.Get()));

	m_pos.x = x;
	m_pos.y = y;

	m_panel = nullptr;
	m_state = false;
	m_direction = 5;
	m_work_num = 0;

	CreateWICTextureFromFile(m_device.Get(), L"Resources\\player.png", nullptr, m_texture[0].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\player1.png", nullptr, m_texture[1].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\player2.png", nullptr, m_texture[2].ReleaseAndGetAddressOf());

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
	m_pos.x -= 0.1f;
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
	//進めるならその方向を返す
	if (m_panel->CanPass(direction) == true)
	{
		return direction;
	}
	else
	{
		return 4;
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
		for (int direction = 3; direction > -1; direction--)
		{
			if (isWork(direction) < 4)
			{
				if (isDirection(direction, m_direction) == true)
				{
					m_state = true;
					m_direction = direction;
					break;
				}
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
		else if (m_direction == RIGHT)
		{
			m_pos.x += 2;
			m_work_num++;
		}
		else
		{
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

//----------------------------------------------------------------------
//! @brief 前回の方向と今から動く方向を比べる関数
//!
//! @param[in] 今の方向、前回の方向
//!
//! @return 前回来た方向と同じ方向ならfalse、同じでないならtrue
//----------------------------------------------------------------------
bool Player::isDirection(int direction, int old_direction)
{
	switch (direction)
	{
	case TOP:
		if (old_direction == BOTTOM)
		{
			return false;
		}
		break;
	case BOTTOM:
		if (old_direction == TOP)
		{
			return false;
		}
		break;
	case LEFT:
		if (old_direction == RIGHT)
		{
			return false;
		}
		break;
	case RIGHT:
		if (old_direction == LEFT)
		{
			return false;
		}
		break;
	default:
		break;
	}

	return true;
}