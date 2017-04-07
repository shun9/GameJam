//************************************************/
//* @file  :Pamel.h
//* @brief :���p�l���̃w�b�_�[
//* @date  :2017/04/07
//* @author:S.Katou
//************************************************/
#pragma once
#include <SpriteBatch.h>
#include <wrl.h>
#include "WICTextureLoader.h"

class Panel
{
	/*--�ÓI�ϐ�--*/
private:
	//�P�ӂ̑傫��
	static int SIZE;



	/*--�ϐ�--*/
private:
	//�摜�f�[�^
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//�X�v���C�g�`��ɕK�{
	std::unique_ptr<DirectX::SpriteBatch> m_sprite;


	//�㉺���E�ɐڂ��Ă���p�l���̃|�C���^
	//Panel*

	//���g���㉺���E�ɓ������邩�ǂ���
	//bool


	/*--�֐�--*/
public:
	Panel(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext
	, const wchar_t* path);

	~Panel() {
		m_sprite.reset();


		m_texture.Reset();
	}

	//�`��
	void Draw(float x, float y);

	//�w������ɐڂ��Ă���p�l����o�^
	void Register(Panel* panel);


	//�w��������i�߂邩�ǂ�����Ԃ�
	bool CanPass();
};