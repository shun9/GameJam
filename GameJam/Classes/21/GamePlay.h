//************************************************/
//* @file  :GamePlay.h
//* @brief :�v���C�V�[��
//* @date  :2017/04/10
//* @author:S.Katou
//************************************************/

#pragma once
#include <vector>
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
	float m_scrollSpd;

	//�}�E�X
	MouseManager* m_mouse;
	int m_mousePosX;
	int m_mousePosY;

	//�f�o�C�X�֘A
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	std::unique_ptr<DirectX::CommonStates> m_commonStates;
	std::unique_ptr<DirectX::SpriteBatch> m_sprite;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_number[10];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_back;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_flame;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_result;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stage;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backToTitle;

	//�X�v���C�g�`��
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	DirectX::SimpleMath::Vector2 m_resultPos;
	DirectX::SimpleMath::Vector2 m_resultPos2;

	//�X�R�A
	int m_score;
	int m_scoreDigit[3];

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
	void DivideScore();

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