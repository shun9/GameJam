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

//�㉺���E
enum Direction
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
};


//�p�l���̒ʉ߉\�������\����
struct PanelPass
{
	bool top;
	bool bottom;
	bool left;
	bool right;
};

//�p�l���N���X
class Panel
{
	/*--�ÓI�ϐ�--*/
private:
	//�P�ӂ̑傫��
	static int SIZE;

	//�p�X�̈ꗗ
	static PanelPass passTopRight;
	static PanelPass passTopLeft;
	static PanelPass passBottomRight;
	static PanelPass passBottomLeft;
	static PanelPass passRightLeft;
	static PanelPass passTopBottom;
	static PanelPass passNone;


	/*--�ϐ�--*/
private:
	//�摜�f�[�^
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//�X�v���C�g�`��ɕK�{
	std::unique_ptr<DirectX::SpriteBatch> m_sprite;


	//�㉺���E�ɐڂ��Ă���p�l���̃|�C���^
	Panel* m_linkPanel[4];

	//���g���㉺���E�ɓ������邩�ǂ���
	bool m_canPass[4];


	/*--�֐�--*/
public:
	//�R���X�g���N�^
	Panel(Microsoft::WRL::ComPtr<ID3D11Device> device
		, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext
		, const wchar_t* path
		, const PanelPass& pass);


	//�f�X�g���N�^
	~Panel();

	//�`��
	void Draw(float x, float y);

	//�w������ɐڂ��Ă���p�l����o�^
	void Register(Panel* panel,int direction);


	//�w��������i�߂邩�ǂ�����Ԃ�
	bool CanPass(int direction);


private:
	//�w������̃p�l���ɐi�߂邩�ǂ�����Ԃ�
	bool CanPassLinkPanel(int direction);
};