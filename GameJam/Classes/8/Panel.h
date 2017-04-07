//************************************************/
//* @file  :Pamel.h
//* @brief :道パネルのヘッダー
//* @date  :2017/04/07
//* @author:S.Katou
//************************************************/
#pragma once


class Panel
{
	/*--静的変数--*/
private:
	//１辺の大きさ
	static int SIZE;



	/*--変数--*/
private:
	//画像データ

	//上下左右に接しているパネルのポインタ
	//Panel*

	//自身が上下左右に道があるかどうか
	//bool


	/*--関数--*/
public:
	Panel() {}
	~Panel() {}

	//描画
	void Draw();

	//指定方向に接しているパネルを登録
	void Register(Panel* panel);


	//指定方向が進めるかどうかを返す
	bool CanPass();
};