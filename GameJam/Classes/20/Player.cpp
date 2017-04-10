//2017�N4��7���@����
//����ҁ@�R�ݐ��s
//�v���C���[�N���X�̃\�[�X�t�@�C��

//2017/04/10
//

#include "Player.h"
#include "..\8\ADX2Le.h"
#include "..\..\Sounds\GamePlaySounds.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;


//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
Player::Player(float x, float y	,Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	:m_device(device)
	,m_deviceContext(context)
{
	m_sprite = std::unique_ptr<DirectX::SpriteBatch>(new SpriteBatch(m_deviceContext.Get()));
	m_commonStates = std::unique_ptr<DirectX::CommonStates>(new CommonStates(m_device.Get()));

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
//! @brief �f�X�g���N�^
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
//! @brief �v���C���[�̏��̍X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::Update(float scroll)
{
	work();
	m_pos.x -= scroll;
}

//----------------------------------------------------------------------
//! @brief �v���C���[�̕`��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::Render()
{
	changeTexture();

	//m_sprite->Begin();
	m_sprite->Begin(SpriteSortMode_Deferred, m_commonStates->NonPremultiplied(), m_commonStates->PointClamp());

	//m_sprite->Draw(m_texture[m_texture_num].Get(), XMFLOAT2(m_pos.x, m_pos.y), nullptr, Colors::White);
	m_sprite->Draw(m_texture[m_texture_num].Get(), XMFLOAT2(m_pos.x, m_pos.y), nullptr, Colors::White, 0.0f, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));

	m_sprite->End();
}

//----------------------------------------------------------------------
//! @brief �v���C���[�������邩�ǂ����Ԃ��֐�
//!
//! @param[in] �Ȃ�
//!
//! @return �v���C���[���ǂ̕����ɓ����邩
//----------------------------------------------------------------------
bool Player::isWork(int direction)
{
	//�i�߂�Ȃ炻�̕�����Ԃ�
	if (m_panel->CanPass(direction) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//----------------------------------------------------------------------
//! @brief �v���C���[���ړ�������֐�
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::work()
{
	if (m_work_num == 1)
	{
		ADX2Le::Play(CRI_GAMEPLAYSOUNDS_DASH_ASPHALT2);
	}

	//�v���C���[�������Ă��Ȃ��ꍇ
	if (m_state == false)
	{
<<<<<<< HEAD
		//�����邩�ǂ������m�F
		for (int direction = 3; direction > -1; direction--)
=======
		m_state = false;
		int dir = m_direction;
		if (isWork(TOP))
>>>>>>> last
		{
			if (isDirection(TOP, dir) == true)
			{
				m_state = true;
				m_direction = TOP;
			}
		}
		if (isWork(BOTTOM))
		{
			if (isDirection(BOTTOM, dir) == true)
			{
				m_state = true;
				m_direction = BOTTOM;
			}
		}
		if (isWork(RIGHT))
		{
			if (isDirection(RIGHT, dir) == true)
			{
				m_state = true;
				m_direction = RIGHT;
			}
		}
	}

	//������ꍇ
	if (m_state == true)
	{
		//�v���C���[������������Ɉړ������A�J�E���g��i�߂�
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


	//�P�p�l�����i�񂾂�
	if (m_work_num >= Panel::SIZE/2)
	{
		//�v���C���[��������Ȃ���Ԃɂ��āA�J�E���g���O�ɂ���
		m_state = false;
		m_work_num = 0;
	}

}

//----------------------------------------------------------------------
//! @brief �v���C���[�̈ʒu�ɂ���p�l����o�^����֐�
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::registerPanel(Panel* panel)
{
	m_panel = panel;
}

//----------------------------------------------------------------------
//! @brief �v���C���[�̉摜��ύX����֐�
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::changeTexture()
{
	//�����Ă��Ȃ��ꍇ
	if (m_state == false)
	{
		m_texture_num = 0;
	}
	
	//������ꍇ
	if (m_state == true)
	{
		if (m_work_num % 20>=10)
		{
			m_texture_num = 1;
		}
		else
		{
			m_texture_num = 2;
		}
	}
}

//----------------------------------------------------------------------
//! @brief �v���C���[�������I�Ɉړ�������֐�
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::compelMove()
{
	//�����Ă��Ȃ���ԂȂ�
	if (m_state == false)
	{
		return;
	}
	//�������������p�l��������菬�����Ȃ�ړ�������
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
//! @brief �O��̕����ƍ����瓮���������ׂ�֐�
//!
//! @param[in] ���̕����A�O��̕���
//!
//! @return �O�񗈂������Ɠ��������Ȃ�false�A�����łȂ��Ȃ�true
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