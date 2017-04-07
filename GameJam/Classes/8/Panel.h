//************************************************/
//* @file  :Pamel.h
//* @brief :���p�l���̃w�b�_�[
//* @date  :2017/04/07
//* @author:S.Katou
//************************************************/
#pragma once


class Panel
{
	/*--�ÓI�ϐ�--*/
private:
	//�P�ӂ̑傫��
	static int SIZE;



	/*--�ϐ�--*/
private:
	//�摜�f�[�^

	//�㉺���E�ɐڂ��Ă���p�l���̃|�C���^
	//Panel*

	//���g���㉺���E�ɓ������邩�ǂ���
	//bool


	/*--�֐�--*/
public:
	Panel() {}
	~Panel() {}

	//�`��
	void Draw();

	//�w������ɐڂ��Ă���p�l����o�^
	void Register(Panel* panel);


	//�w��������i�߂邩�ǂ�����Ԃ�
	bool CanPass();
};