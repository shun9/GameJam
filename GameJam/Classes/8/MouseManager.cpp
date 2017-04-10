//************************************************/
//* @file  :Mouse.cpp
//* @brief :�}�E�X�֘A�̃N���X
//* @date  :2017/04/09
//* @author:S.Katou
//************************************************/
#include "MouseManager.h"


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�X�V����
//�b����  :�܂�(void)
//�b�߂�l:�Ȃ�(void)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
void MouseManager::Update()
{
	m_state = m_mouse->GetState();
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :���̃{�^����������Ă��邩�ǂ���
//�b����  :�܂�(void)
//�b�߂�l:������Ă�����true(bool)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
bool MouseManager::IsClickedLeft()
{
	return m_state.leftButton;
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�}�E�X�̂w���W��Ԃ�
//�b����  :�܂�(void)
//�b�߂�l:�w���W(int)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
int MouseManager::GetPosX()
{
	return m_state.x;
}

//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�}�E�X�̂x���W��Ԃ�
//�b����  :�܂�(void)
//�b�߂�l:�x���W(int)
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
int MouseManager::GetPosY()
{
	return m_state.y;
}


//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
//�b�@�\  :�C���X�^���X�̐����@�擾(�V���O���g��)
//�b����  :�Ȃ�(void)
//�b�߂�l:�C���X�^���X(MouseManager*)	
//�{�[�[�[�[�[�[�[�[�[�[�[�[�[�[�{
MouseManager* MouseManager::GetInstance()
{
	static MouseManager instance;
	return &instance;
}


/*--�ȉ��B��--*/
MouseManager::MouseManager()
{
	m_mouse = std::make_unique<DirectX::Mouse>();
}

MouseManager::~MouseManager()
{
}
