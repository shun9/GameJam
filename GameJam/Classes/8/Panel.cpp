//************************************************/
//* @file  :Pamel.cpp
//* @brief :���p�l���̃\�[�X�t�@�C��
//* @date  :2017/04/09
//* @author:S.Katou
//************************************************/
#include "Panel.h"

#include <random>
#include <SimpleMath.h>

using namespace DirectX;

//�P�ӂ̑傫��
const int Panel::SIZE         = 128;
const int Panel::MAX_PASS_NUM = 7;

//�p�X�ꗗ                                 ��@�@���@�@���@ �E
const PanelPass Panel::passTopRight    = { true, false,false,true,  L"Resources\\Road3.png" };
const PanelPass Panel::passTopLeft     = { true, false,true, false, L"Resources\\Road4.png" };
const PanelPass Panel::passBottomRight = { false,true, false,true,  L"Resources\\Road5.png" };
const PanelPass Panel::passBottomLeft  = { false,true, true, false, L"Resources\\Road6.png" };
const PanelPass Panel::passRightLeft   = { false,false,true, true,  L"Resources\\Road1.png" };
const PanelPass Panel::passTopBottom   = { true, true, false,false, L"Resources\\Road2.png" };
const PanelPass Panel::passNone        = { false,false,false,false, L"Resources\\Road7.png" };

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�p�l���̃p�X�������_���ɕԂ�
//�b      :�����_�������p
//�b����  :�Ȃ�(void)
//�b�߂�l:�p�X�\����(PanelPass)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
PanelPass Panel::GetRandomPass()
{
	//���������p
	std::random_device random;
	std::mt19937 mt(random());

	//�����̒l�𐧌�
	std::uniform_int_distribution<int> num(0, 100);
	
	PanelPass pass;

	int result = num(mt);

	if		(result < 10) { pass = passRightLeft;	}
	else if (result < 26) { pass = passTopRight;	}
	else if (result < 42) { pass = passTopLeft;		}
	else if (result < 58) { pass = passBottomRight;	}
	else if (result < 74) { pass = passBottomLeft;	}
	else if (result < 90) { pass = passTopBottom;	}
	else				  { pass = passNone;		}

	return pass;
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�R���X�g���N�^
//�b����  :�f�o�C�X				(ComPtr<ID3D11Device>)
//�b����  :�f�o�C�X�R���e�L�X�g	(ComPtr<ID3D11DeviceContext>)
//�b����  :�t�@�C���p�X			(wchar_t*)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
Panel::Panel(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext
	, const PanelPass& pass)
{
	//�X�v���C�g�o�b�`�̐ݒ�
	m_sprite = std::make_unique<SpriteBatch>((deviceContext.Get()));

	//�摜�̓ǂݍ���
	CreateWICTextureFromFile(device.Get(), pass.path, nullptr, m_texture.ReleaseAndGetAddressOf());


	//�������������ݒ�
	m_canPass[TOP]    = pass.top;
	m_canPass[BOTTOM] = pass.bottom;
	m_canPass[LEFT]   = pass.left;
	m_canPass[RIGHT]  = pass.right;

	//�אڂ���p�l�� �㉺���E
	for (int i = 0; i < 4; i++)
	{
		m_linkPanel[i] = nullptr;
	}
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�f�X�g���N�^
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
Panel::~Panel()
{
	m_sprite.reset();
	m_texture.Reset();
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�`��
//�b����  :�`����W�w(float)
//�b����  :�`����W�x(float)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void Panel::Draw(float x, float y)
{
	m_sprite->Begin();

	//�`��@128.0f�͌��摜�̃T�C�Y�ł�
	m_sprite->Draw(m_texture.Get(), XMFLOAT2(x, y), nullptr, Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), SIZE / 128.0f);

	m_sprite->End();
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�אڂ���p�l����o�^����
//�b����  :�o�^����p�l��(Panel*)
//�b����  :�אڂ�������@(int)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void Panel::Register(Panel* panel, int direction)
{
	m_linkPanel[direction] = panel;
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�w�肳�ꂽ�����ɐi�߂邩�ǂ���
//�b����  :����(int)
//�b�߂�l:�i�߂���true(bool)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
bool Panel::CanPass(int direction)
{
	//���g�ɓ���������ΐi�߂Ȃ�
	if (m_canPass[direction] == false)
	{
		return false;
	}


	//�w������Ƀp�l����������ΐi�߂Ȃ�
	if (m_linkPanel[direction] == nullptr)
	{
		return true;
	}

	//�אڂ���p�l���ɓ���������ΐi�߂Ȃ�
	if (CanPassLinkPanel(direction) == false)
	{
		return false;
	}

	//���Ȃ���ΐi�߂�
	return true;
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�w�肳�ꂽ�����̃p�l���ɐi�߂邩�ǂ���
//�b����  :����(int)
//�b�߂�l:�i�߂���true(bool)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
bool Panel::CanPassLinkPanel(int direction)
{

	bool canPass = false;

	//�i�s�����Ƒ΂ɂȂ�ʒu�̓��𒲂ׂ�
	switch (direction)
	{
	case TOP:
		canPass = m_linkPanel[direction]->m_canPass[BOTTOM];
		break;
	case BOTTOM:
		canPass = m_linkPanel[direction]->m_canPass[TOP];
		break;

	case LEFT:
		canPass = m_linkPanel[direction]->m_canPass[RIGHT];
		break;
	case RIGHT:
		canPass = m_linkPanel[direction]->m_canPass[LEFT];
		break;

	default:
		break;
	}

	return canPass;
}


