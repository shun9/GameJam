//--------------------------------------------------------------------------------------
// File: GamePlay.h
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#pragma once
#include <vector>
#include <SpriteBatch.h>
#include <wrl.h>
#include "WICTextureLoader.h"
#include "GameScene.h"
#include "..\8\Panel.h"

#include "../8/MouseManager.h"

#include "../20/Player.h"

struct OPTION
{
	Panel* panel;
	float posX;
	float posY;
};

class GamePlay : public GameScene
{
	/*--�ÓI�ϐ�--*/
public:
	static const int MAP_Y;
	static const int MAP_X;
	static const int MAP_POS_X;
	static const int MAP_POS_Y;

private:
	static const int MAX_OPTION;
	static const float OPTION_POS_X[3];
	static const float OPTION_POS_Y;


	/*--�����o�ϐ�--*/
private:
	//�v���C���[
	Player* m_player;

	//�X�e�[�W�̃p�l��
	Panel*** m_panel;
	
	//�I�����̃p�l��
	OPTION* m_option;

	//�I�𒆂̔ԍ�
	int m_numChoosed;

	//�J�E���g
	int m_cntTime;
	float m_scrollPos;

	//�}�E�X
	MouseManager* m_mouse;
	int m_mousePosX;
	int m_mousePosY;

	//�f�o�C�X�֘A
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_result;

	//�X�v���C�g�`��
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//�X�R�A
	int m_score;

	//�Q�[���I�[�o�[�t���O
	bool m_isGameOver;

	/*--�����o�֐�--*/
public:
	GamePlay(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	~GamePlay();

	void Update();
	void Render();

private:
	//�X�V�����֘A
	void UpdateStage();
	void UpdateOption();
	void UpdatePlayer();

	//�Q�[���V�X�e���֘A
	void FitOption();
	void PanelSlide();
	bool IsDead();
	void GameOver();
	
	//�I�񂾑I�������擾����
	int ChoosedOption();

	//�p�l�����q����
	void LinkPanel();

	//�`��֘A
	void DrawStage();
	void DrawOption();

	//�쐬�֘A
	void CreateStage();
	void CreateOption();

	//�폜�֘A
	void DeleteStage();
	void DeleteOption();
};