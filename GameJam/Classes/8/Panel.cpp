//************************************************/
//* @file  :Pamel.cpp
//* @brief :���p�l���̃\�[�X�t�@�C��
//* @date  :2017/04/07
//* @author:S.Katou
//************************************************/
#include "Panel.h"

using namespace DirectX;

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :���e
//�b����  :���e(�^��)
//�b�߂�l:���e(�^��)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
Panel::Panel(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext
	, const wchar_t* path)
{

	m_sprite = std::make_unique<SpriteBatch>((deviceContext.Get()));

	CreateWICTextureFromFile(device.Get(), path, nullptr, m_texture.ReleaseAndGetAddressOf());
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


