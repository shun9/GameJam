//************************************************/
//* @file  :Pamel.h
//* @brief :���p�l���̃w�b�_�[
//* @date  :2017/04/09
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
	wchar_t* path;
};

//�p�l���N���X
class Panel
{
	/*--�ÓI�ϐ�--*/
public:
	//�P�ӂ̑傫��
	static const int SIZE;
	static const int MAX_PASS_NUM;

	//�p�X�̈ꗗ
	static const PanelPass passTopRight;
	static const PanelPass passTopLeft;
	static const PanelPass passBottomRight;
	static const PanelPass passBottomLeft;
	static const PanelPass passRightLeft;
	static const PanelPass passTopBottom;
	static const PanelPass passNone;

	/*--�ÓI�֐�--*/
public:
	//�����_���ȃp�l���̃p�X��Ԃ�
	static PanelPass GetRandomPass();


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