//************************************************/
//* @file  :Pamel.cpp
//* @brief :���p�l���̃\�[�X�t�@�C��
//* @date  :2017/04/07
//* @author:S.Katou
//************************************************/
#include "Panel.h"

using namespace DirectX;

//�P�ӂ̑傫��
int Panel::SIZE = 64;

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�R���X�g���N�^
//�b����  :�f�o�C�X				(ComPtr<ID3D11Device>)
//�b����  :�f�o�C�X�R���e�L�X�g	(ComPtr<ID3D11DeviceContext>)
//�b����  :�t�@�C���p�X			(wchar_t*)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
Panel::Panel(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext
	, const wchar_t* path
	, const PanelPass& pass)
{
	//�X�v���C�g�o�b�`�̐ݒ�
	m_sprite = std::make_unique<SpriteBatch>((deviceContext.Get()));

	//�摜�̓ǂݍ���
	CreateWICTextureFromFile(device.Get(), path, nullptr, m_texture.ReleaseAndGetAddressOf());


	//�������������ݒ�
	m_canPass[TOP] = pass.top;
	m_canPass[BOTTOM] = pass.bottom;
	m_canPass[LEFT] = pass.left;
	m_canPass[RIGHT] = pass.right;

	//�אڂ���p�l��
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

	m_sprite->Draw(m_texture.Get(), XMFLOAT2(x, y), nullptr, Colors::White);

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
	if (!m_canPass[direction])
	{
		return false;
	}


	//�w������Ƀp�l����������ΐi�߂Ȃ�
	if (m_linkPanel[direction] == nullptr)
	{
		return false;
	}

	//�אڂ���p�l���ɓ���������ΐi�߂Ȃ�
	if (!CanPassLinkPanel(direction))
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
	case BOTTOM:
		canPass = m_linkPanel[direction]->m_canPass[TOP];

	case LEFT:
		canPass = m_linkPanel[direction]->m_canPass[LEFT];
	case RIGHT:
		canPass = m_linkPanel[direction]->m_canPass[RIGHT];

	default:
		break;
	}

	return canPass;
}


