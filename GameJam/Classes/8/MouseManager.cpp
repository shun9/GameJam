//************************************************/
//* @file  :Mouse.cpp
//* @brief :マウス関連のクラス
//* @date  :2017/04/09
//* @author:S.Katou
//************************************************/
#include "MouseManager.h"


//＋ーーーーーーーーーーーーーー＋
//｜機能  :更新処理
//｜引数  :まし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void MouseManager::Update()
{
	m_state = m_mouse->GetState();
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :左のボタンが押されているかどうか
//｜引数  :まし(void)
//｜戻り値:押されていたらtrue(bool)
//＋ーーーーーーーーーーーーーー＋
bool MouseManager::IsClickedLeft()
{
	return m_state.leftButton;
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :マウスのＸ座標を返す
//｜引数  :まし(void)
//｜戻り値:Ｘ座標(int)
//＋ーーーーーーーーーーーーーー＋
int MouseManager::GetPosX()
{
	return m_state.x;
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :マウスのＹ座標を返す
//｜引数  :まし(void)
//｜戻り値:Ｙ座標(int)
//＋ーーーーーーーーーーーーーー＋
int MouseManager::GetPosY()
{
	return m_state.y;
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :インスタンスの生成　取得(シングルトン)
//｜引数  :なし(void)
//｜戻り値:インスタンス(MouseManager*)	
//＋ーーーーーーーーーーーーーー＋
MouseManager* MouseManager::GetInstance()
{
	static MouseManager instance;
	return &instance;
}


/*--以下隠蔽--*/
MouseManager::MouseManager()
{
	m_mouse = std::make_unique<DirectX::Mouse>();
}

MouseManager::~MouseManager()
{
}
