//--------------------------------------------------------------------------------------
// File: GamePlay.cpp
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#include "..\..\pch.h"
#include "..\..\Game.h"
#include "GamePlay.h"

#include "..\8\ADX2Le.h"
#include "..\..\Sounds\GamePlaySounds.h"


using namespace std;

const int GamePlay::MAP_Y             = 3;
const int GamePlay::MAP_X             = 8;
const int GamePlay::MAP_POS_X         = 0;
const int GamePlay::MAP_POS_Y         = 20;
const int GamePlay::MAX_OPTION        = 3;
const float GamePlay::OPTION_POS_X[3] = {200.0f,350.0f,500.0f};
const float GamePlay::OPTION_POS_Y    = 450.0f;

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�R���X�g���N�^
//�b����  :�f�o�C�X		(Microsoft::WRL::ComPtr<ID3D11Device>)
//�b����  :�R���e�L�X�g (Microsoft::WRL::ComPtr<ID3D11DeviceContext>)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
GamePlay::GamePlay(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	: m_device(device)
	, m_context(context)
	, m_cntTime(0)
	, m_scrollPos(0)
	, m_numChoosed(-1)
	, m_score(0)
	, m_isGameOver(false)
{	
	//�T�E���h�̏�����
	ADX2Le::LoadAcb("Sounds\\GamePlaySounds.acb", "Sounds\\GamePlaySounds.awb");
	ADX2Le::Play(CRI_GAMEPLAYSOUNDS__CUE_ID_1);

	m_spriteBatch = std::make_unique<SpriteBatch>(context.Get());
	CreateWICTextureFromFile(device.Get(), L"Resources\\title.png", nullptr, m_background.ReleaseAndGetAddressOf());

	//���̃V�[��
	m_next = PLAY;

	//�}�E�X�̎擾
	m_mouse = MouseManager::GetInstance();

	//�X�e�[�W�̍쐬
	CreateStage();

	//�I�����̍쐬
	CreateOption();

	m_player = new Player(MAP_POS_X+Panel::SIZE,
						  MAP_POS_Y+Panel::SIZE,
						  m_device,m_context);
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�f�X�g���N�^
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
GamePlay::~GamePlay()
{
	//�X�e�[�W�폜
	DeleteStage();

	//�I�����폜
	DeleteOption();
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�X�V����
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::Update()
{
	//�}�E�X�X�V
	m_mouse->Update();

	//�}�E�X�̍��W���X�V �Ԃɋ��ނ��ߔ������炷
	m_mousePosX = (m_mouse->GetPosX() + m_scrollPos+ (Panel::SIZE / 2) - MAP_POS_X) / Panel::SIZE ;
	m_mousePosY = (m_mouse->GetPosY()-MAP_POS_Y) / Panel::SIZE;

	//�X�e�[�W�ړ�
	UpdateStage();

	if (m_isGameOver)
	{
		GameOver();
		return;
	}
	//�I�����̍X�V
	UpdateOption();

	//�v���C���[�̍X�V
	UpdatePlayer();

}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�`�揈��
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::Render()
{
	//�X�e�[�W�̕`��
	DrawStage();

	//�I�����̕`��
	DrawOption();

	//�v���C���[�`��
	m_player->Render();

	if (m_isGameOver)
	{
		m_spriteBatch->Begin();
		m_spriteBatch->Draw(m_button.Get(), Vector2(m_fullscreenRect.right / 4.0f, m_fullscreenRect.bottom / 1.5f),
			nullptr, Colors::White, 0.f, m_buttonorigin);
		m_spriteBatch->End();
	}
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�X�e�[�W�̍X�V
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::UpdateStage()
{
	//�X�N���[������
	m_scrollPos += 0.1f;

	//�P�p�l�����X�N���[��������
	if (m_scrollPos >= Panel::SIZE)
	{
		//�p�l�������炷
		PanelSlide();

		//�X�R�A�����Z
		m_score++;

		//�ʒu��߂�
		m_scrollPos = 0.0f;
	}
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�I�����̍X�V
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::UpdateOption()
{
	if (m_mouse->IsClickedLeft())
	{
		if (m_numChoosed <= -1)
		{
			//�I�������Ƃ�
			m_numChoosed = ChoosedOption();
		}
	}
	else
	{
		//�I������I��ł���ꍇ
		if (m_numChoosed >= 0)
		{
			FitOption();
		}

		//�I���������Z�b�g
		m_numChoosed = -1;
	}
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�v���C���[�̍X�V
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::UpdatePlayer()
{
	m_player->Update();

	DirectX::SimpleMath::Vector2 pos = m_player->getPos();
	int x = (pos.x + m_scrollPos - MAP_POS_X) / Panel::SIZE;
	int y = (pos.y - MAP_POS_Y) / Panel::SIZE;

	m_player->registerPanel(m_panel[y][x]);
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�p�l�����Z�b�g����
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::FitOption()
{
	//�X�e�[�W�O�Ȃ�Δ�����
	if (m_mousePosX < 0 || m_mousePosX >= MAP_X
	||  m_mousePosY < 0 || m_mousePosY >= MAP_Y)
	{
		return;
	}

	//�Ō�����폜
	delete m_panel[m_mousePosY][MAP_X-1];

	//�p�l�������炷
	int i = MAP_X - 1;
	while (i != m_mousePosX)
	{
		m_panel[m_mousePosY][i] = m_panel[m_mousePosY][i - 1];
		i--;
	}
	//�p�l�����͂߂�
	m_panel[m_mousePosY][m_mousePosX] = m_option[m_numChoosed].panel;

	//���ʉ��Đ�
	ADX2Le::Play(CRI_GAMEPLAYSOUNDS__CUE_ID_0);

	//�V�����I�������쐬
	m_option[m_numChoosed].panel = new Panel(m_device, m_context, Panel::GetRandomPass());

	LinkPanel();
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�p�l�������炷
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::PanelSlide()
{
	for (int i = 0; i < MAP_Y; i++)
	{
		//�[�̃p�l��������
		delete m_panel[i][0];

		for (int j = 0; j < MAP_X-1; j++)
		{
			//�p�l�������炷
			m_panel[i][j] = m_panel[i][j + 1];
		}
		
		//�Ō���ɐV�����p�l�����쐬
		m_panel[i][MAP_X - 1] = new Panel(m_device, m_context, Panel::GetRandomPass());
	}

	LinkPanel();
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�Q�[���I�[�o�[����
//�b����  :�Ȃ�(void)
//�b�߂�l:�Q�[���I�[�o�[��true(bool)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
bool GamePlay::IsDead()
{
	DirectX::SimpleMath::Vector2 pos = m_player->getPos();

	if (pos.x < -Panel::SIZE / 2)
	{

	}
	return 0;
}

void GamePlay::GameOver()
{
	if (m_mouse->IsClickedLeft())
	{
		m_next = TITLE;
	}
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�}�E�X�Ɛڂ��Ă���I������Ԃ�
//�b����  :�Ȃ�(void)
//�b�߂�l:�I�����̔ԍ�(int)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
int GamePlay::ChoosedOption()
{
	//�}�E�X�̍��W
	int mouseX = m_mouse->GetPosX();
	int mouseY = m_mouse->GetPosY();

	//�I�����ԍ�
	int option = -1;

	for (int i = 0; i < MAX_OPTION; i++)
	{
		//�}�E�X���I�����ɐG��Ă����ꍇ�@������
		if (m_option[i].posX               <= mouseX 
		&&  m_option[i].posX + Panel::SIZE >= mouseX
		&&  m_option[i].posY			   <= mouseY
		&&  m_option[i].posY + Panel::SIZE >= mouseY)
		{
			option = i;
			break;
		}
	}

	return option;
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�X�e�[�W�̃p�l����ڑ�����
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::LinkPanel()
{
	//�p�l����ڑ�����
	for (int i = 0; i < MAP_Y; i++)
	{
		for (int j = 0; j < MAP_X; j++)
		{
			//��̃p�l����ڑ�
			if (i - 1 >= 0)
			{
				m_panel[i][j]->Register(m_panel[i - 1][j], TOP);
			}

			//���̃p�l����ڑ�
			if (i + 1 < MAP_Y)
			{
				m_panel[i][j]->Register(m_panel[i + 1][j], BOTTOM);
			}

			//���̃p�l����ڑ�
			if (j - 1 >= 0)
			{
				m_panel[i][j]->Register(m_panel[i][j-1], LEFT);
			}

			//�E�̃p�l����ڑ�
			if (j + 1 < MAP_X)
			{
				m_panel[i][j]->Register(m_panel[i][j+1], RIGHT);
			}
		}
	}
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�X�e�[�W�̕`��
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::DrawStage()
{
	//�X�e�[�W�̕`��
	for (int i = 0; i < MAP_Y; i++)
	{
		for (int j = 0; j < MAP_X; j++)
		{
			m_panel[i][j]->Draw(j*Panel::SIZE + MAP_POS_X - m_scrollPos, i*Panel::SIZE + MAP_POS_Y);
		}
	}
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�I�����̕`��
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::DrawOption()
{
	//�I�����̕`��
	for (int i = 0; i < MAX_OPTION; i++)
	{
		if (m_numChoosed == i)
		{
			m_option[i].panel->Draw(m_mouse->GetPosX() - (Panel::SIZE / 2.0f),
				m_mouse->GetPosY() - (Panel::SIZE / 2.0f));
		}
		else
		{
			m_option[i].panel->Draw(m_option[i].posX, m_option[i].posY);
		}
	}
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�X�e�[�W���쐬����
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::CreateStage()
{
	// �X�e�[�W���Q���z��œ��I�m��
	m_panel = new Panel**[MAP_Y];
	for (int i = 0; i < MAP_Y; i++)
	{
		m_panel[i] = new Panel*[MAP_X];
	}

	//�X�e�[�W�Ƀp�l����ݒ�
	for (int i = 0; i < MAP_Y; i++)
	{
		for (int j = 0; j < MAP_X; j++)
		{
			m_panel[i][j] = new Panel(m_device, m_context, Panel::GetRandomPass());
		}
	}

	//�p�l����ڑ�����
	LinkPanel();
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�I�������쐬����
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::CreateOption()
{
	m_option = new OPTION[MAX_OPTION];

	for (int i = 0; i < MAX_OPTION; i++)
	{
		m_option[i].panel = new Panel(m_device, m_context, Panel::GetRandomPass());
		m_option[i].posX = OPTION_POS_X[i];
		m_option[i].posY = OPTION_POS_Y;
	}
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�I�������폜����
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::DeleteStage()
{
	//�p�l���̍폜
	for (int i = 0; i < MAP_Y; i++)
	{
		for (int j = 0; j < MAP_X; j++)
		{
			delete m_panel[i][j];
		}
	}

	for (int i = 0; i < MAP_Y; i++)
	{
		delete m_panel[i];
	}

	delete m_panel;
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�I�������폜����
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::DeleteOption()
{
	for (int i = 0; i < MAX_OPTION; i++)
	{
		delete m_option[i].panel;
	}
	delete[] m_option;
}
