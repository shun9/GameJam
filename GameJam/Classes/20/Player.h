//2017�N4��7���@����
//����ҁ@�R�ݐ��s
//�v���C���[�N���X�̃w�b�_�[�t�@�C��

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include <wrl.h>
#include <..\..\Users\s162188\Documents\GameJam\GameJam\Classes\8\Panel.h>

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

public:
	Player(float x, float y);
	~Player();
	void Update();
	void Render();
	void registerPanel(Panel* panel);	//�v���C���[�̈ʒu�ɂ���p�l����o�^����֐�
};
