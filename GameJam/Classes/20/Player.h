//2017�N4��7���@����
//����ҁ@�R�ݐ��s
//�v���C���[�N���X�̃w�b�_�[�t�@�C��

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include <wrl.h>
<<<<<<< HEAD
#include "../8/Panel.h"
=======
<<<<<<< HEAD
#include "..\8\Panel.h"
=======
#include "../8/Panel.h"
>>>>>>> 33cf137ad88dc2d60e5d131307422f78894dae9a
>>>>>>> 3899fc73ce62e7b6130a7e3b3a399583b1eeb0c6

class Player
{
private:
	std::unique_ptr<DirectX::SpriteBatch> m_sprite;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture[3];
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_pos;		//���݂̈ʒu
	int m_direction;					//�v���C���[�̓�������
	int m_work_num;						//�v���C���[������������
	int m_texture_num;					//���݂̉摜
	bool m_state;						//�v���C���[�������邩�̏��

	Panel* m_panel;						//�v���C���[������Ă���p�l���̏��
	int isWork(int direction);			//�v���C���[���ǂ̕����ɓ����邩��Ԃ��֐�
	void work();						//�v���C���[�𓮂����֐�
	void changeTexture();				//�v���C���[�̉摜��ύX����֐�
	void compelMove();					//�v���C���[�������I�Ɉړ�������֐�

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;


	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;

public:
<<<<<<< HEAD
	Player(float x, float y,
=======
	Player(float x, float y, 
>>>>>>> 3899fc73ce62e7b6130a7e3b3a399583b1eeb0c6
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	~Player();
	void Update();
	void Render();
	void registerPanel(Panel* panel);	//�v���C���[�̈ʒu�ɂ���p�l����o�^����֐�

	DirectX::SimpleMath::Vector2 getPos() { return m_pos; }
<<<<<<< HEAD
};
=======
};
>>>>>>> 3899fc73ce62e7b6130a7e3b3a399583b1eeb0c6
