//2017�N4��7���@����
//����ҁ@�R�ݐ��s
//�v���C���[�N���X�̃\�[�X�t�@�C��

#include "Player.h"

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
void Player::Update()
{
	work();
	m_pos.x -= 0.1f;
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
	m_sprite->Begin();

	m_sprite->Draw(m_texture[m_texture_num].Get(), XMFLOAT2(m_pos.x, m_pos.y), nullptr, Colors::White);

	m_sprite->End();
}

//----------------------------------------------------------------------
//! @brief �v���C���[�������邩�ǂ����Ԃ��֐�
//!
//! @param[in] �Ȃ�
//!
//! @return �v���C���[���ǂ̕����ɓ����邩
//----------------------------------------------------------------------
int Player::isWork(int direction)
{
	//�i�߂�Ȃ炻�̕�����Ԃ�
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
//! @brief �v���C���[���ړ�������֐�
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::work()
{
	//�v���C���[�������Ă��Ȃ��ꍇ
	if (m_state == false)
	{
		//�����邩�ǂ������m�F
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
	if (m_work_num == 64)
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
	else if (m_state == true)
	{
		//�����Ă�������ŉ摜�������ւ���
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