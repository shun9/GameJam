//************************************************/
//* @file  :Mouse.h
//* @brief :マウス関連のクラス
//* @date  :2017/04/09
//* @author:S.Katou
//************************************************/
#pragma once
#include <Windows.h>
#include <Mouse.h>

class MouseManager
{
public:
	static MouseManager* GetInstance();

private:
	std::unique_ptr<DirectX::Mouse>m_mouse;
	DirectX::Mouse::State m_state;

public:
	void Update();

	bool IsClickedLeft();

	int GetPosX();
	int GetPosY();
private:
	MouseManager();
	~MouseManager();
};