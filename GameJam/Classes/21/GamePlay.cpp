//************************************************/
//* @file  :GamePlay.cpp
//* @brief :�v���C�V�[��
//* @date  :2017/04/10
//* @author:S.Katou
//************************************************/

#include "..\..\pch.h"
#include "..\..\Game.h"
#include "GamePlay.h"

#include "..\8\ADX2Le.h"
#include "..\..\Sounds\GamePlaySounds.h"


using namespace std;
using namespace DirectX;

const int GamePlay::MAP_Y             = 3;
const int GamePlay::MAP_X             = 8;
const int GamePlay::MAP_POS_X         = 0;
const int GamePlay::MAP_POS_Y         = 20;
const int GamePlay::MAX_OPTION        = 3;
const float GamePlay::OPTION_POS_X[3] = {158.0f,343.0f,521.0f};
const float GamePlay::OPTION_POS_Y    = 455.0f;

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
	, m_scrollSpd(0.2f)
	, m_numChoosed(-1)
	, m_score(0)
	, m_isGameOver(false)
{	

	//���̃V�[��
	m_next = PLAY;

	ADX2Le::LoadAcb("Sounds\\GamePlaySounds.acb", "Sounds\\GamePlaySounds.awb");
	ADX2Le::Play(CRI_GAMEPLAYSOUNDS_CONVEYOR);

	//�}�E�X�̎擾
	m_mouse = MouseManager::GetInstance();


	m_sprite = unique_ptr<SpriteBatch>(new SpriteBatch(m_context.Get()));
	m_commonStates = unique_ptr<CommonStates>(new CommonStates(m_device.Get()));
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\Road7.png", nullptr, m_back.ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\Result.png", nullptr, m_result.ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\BacktoTitle.png", nullptr, m_backToTitle.ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\stage.png", nullptr, m_stage.ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\flame.png", nullptr, m_flame.ReleaseAndGetAddressOf());

	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number0.png", nullptr, m_number[0].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number1.png", nullptr, m_number[1].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number2.png", nullptr, m_number[2].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number3.png", nullptr, m_number[3].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number4.png", nullptr, m_number[4].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number5.png", nullptr, m_number[5].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number6.png", nullptr, m_number[6].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number7.png", nullptr, m_number[7].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number8.png", nullptr, m_number[8].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number9.png", nullptr, m_number[9].ReleaseAndGetAddressOf());

	//�X�e�[�W�̍쐬
	CreateStage();

	//�I�����̍쐬
	CreateOption();

	m_player = new Player(MAP_POS_X+Panel::SIZE+20,
						  MAP_POS_Y+Panel::SIZE+MAP_POS_Y,
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
	
	//�Q�[���i�s���̏���
	if (!m_isGameOver)
	{

	//�}�E�X�̍��W���X�V �Ԃɋ��ނ��ߔ������炷
	m_mousePosX = (m_mouse->GetPosX() + m_scrollPos+ (Panel::SIZE / 2) - MAP_POS_X) / Panel::SIZE ;
	m_mousePosY = (m_mouse->GetPosY()-MAP_POS_Y) / Panel::SIZE;

	//�X�e�[�W�ړ�
	UpdateStage();

	//�I�����̍X�V
	UpdateOption();

	//�v���C���[�̍X�V
	UpdatePlayer();

	//�Q�[���I�[�o�[����
	m_isGameOver = IsDead();
	}

	//�Q�[���I�[�o�[���̏���
	if (m_isGameOver)
	{
		m_cntTime++;
		GameOver();
	}
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�`�揈��
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::Render()
{
	//�`��J�n
	m_sprite->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied(), m_commonStates->PointClamp());
	
	//�w�i�`��
	m_sprite->Draw(m_back.Get(), DirectX::XMFLOAT2(0.0f, 0.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(10.0f, 3.44f));
	
	//�t���[���`��
	m_sprite->Draw(m_flame.Get(), DirectX::XMFLOAT2(0.0f, 440.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));

	m_sprite->End();


	//�X�e�[�W�̕`��
	DrawStage();

	//�`��J�n
	m_sprite->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied(), m_commonStates->PointClamp());
	//�Ƃ��`��
	m_sprite->Draw(m_stage.Get(), DirectX::XMFLOAT2(-12.0f, -1.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.05f, 1.1f));
	m_sprite->End();


	//�I�����̕`��
	DrawOption();

	//�v���C���[�`��
	m_player->Render();



	//�`��J�n
	m_sprite->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied(), m_commonStates->PointClamp());

	if (m_isGameOver)
	{
		//���U���g���
		m_sprite->Draw(m_result.Get(), DirectX::XMFLOAT2(100.0f, 70.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
		m_sprite->Draw(m_backToTitle.Get(), DirectX::XMFLOAT2(180.0f, 450.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
		
		//�X�R�A
		m_sprite->Draw(m_number[m_scoreDigit[0]].Get(), DirectX::XMFLOAT2(200.0f, 200.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(3.0f, 3.0f));
		m_sprite->Draw(m_number[m_scoreDigit[1]].Get(), DirectX::XMFLOAT2(350.0f, 200.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(3.0f, 3.0f));
		m_sprite->Draw(m_number[m_scoreDigit[2]].Get(), DirectX::XMFLOAT2(500.0f, 200.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(3.0f, 3.0f));
	}

	m_sprite->End();

}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�X�e�[�W�̍X�V
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::UpdateStage()
{
	//�X�N���[������
	m_scrollPos += m_scrollSpd;

	//�P�p�l�����X�N���[��������
	if (m_scrollPos >= Panel::SIZE)
	{
		//�p�l�������炷
		PanelSlide();

		//�X�R�A�����Z
		m_score++;

		//�ʒu��߂�
		m_scrollPos = 0.0f;

		//�X�s�[�h�A�b�v
		m_scrollSpd += 0.01f;
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
	DirectX::SimpleMath::Vector2 pos = m_player->getPos();
	int x = (pos.x +m_scrollPos - MAP_POS_X) / Panel::SIZE;
	int y = (pos.y - MAP_POS_Y) / Panel::SIZE;

	m_player->registerPanel(m_panel[y][x]);

	m_player->Update(m_scrollSpd);
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

	//�X�e�[�W�O�ɏo����Q�[���I�[�o�[
	if (pos.x < 0.0f
	||  pos.x > 740.0f          
	||  pos.y < 0.0f
	||  pos.y >  Panel::SIZE * MAP_Y-30.0f)
	{
		ADX2Le::LoadAcb("Sounds\\GamePlaySounds.acb", "Sounds\\GamePlaySounds.awb");
		ADX2Le::Play(CRI_GAMEPLAYSOUNDS__CUE_ID_7);
		DivideScore();
		return true;
	}

	return false;
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�Q�[���I�[�o�[����
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::GameOver()
{
	if (m_cntTime >= 60)
	{
		if (m_mouse->IsClickedLeft())
		{
			m_next = TITLE;
		}
	}
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�X�R�A�������Ƃɕ�������
//�b����  :�Ȃ�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void GamePlay::DivideScore()
{
	//1000�ŃJ���X�g
	if (m_score >= 1000)
	{
		m_score = 999;
		m_scoreDigit[0] = 9;
		m_scoreDigit[1] = 9;
		m_scoreDigit[2] = 9;
		return;
	}
	
	//100�̈�
	int digit100 = 0;
	while (m_score >= 100)
	{
		digit100++;
		m_score -= 100;
	}
	m_scoreDigit[0] = digit100;

	//10�̈�
	int digit10 = 0;
	while (m_score >= 10)
	{
		digit10++;
		m_score -= 10;
	}
	m_scoreDigit[1] = digit10;

	//1�̈�
	int digit1 = 0;
	while (m_score >= 1)
	{
		digit1++;
		m_score -= 1;
	}

	m_scoreDigit[2] = digit1;

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
