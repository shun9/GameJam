//************************************************/
//* @file  :Pamel.cpp
//* @brief :道パネルのソースファイル
//* @date  :2017/04/07
//* @author:S.Katou
//************************************************/
#include "Panel.h"

using namespace DirectX;

//＋ーーーーーーーーーーーーーー＋
//｜機能  :内容
//｜引数  :内容(型名)
//｜戻り値:内容(型名)
//＋ーーーーーーーーーーーーーー＋
Panel::Panel(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext
	, const wchar_t* path)
{

	m_sprite = std::make_unique<SpriteBatch>((deviceContext.Get()));

	CreateWICTextureFromFile(device.Get(), path, nullptr, m_texture.ReleaseAndGetAddressOf());
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :描画
//｜引数  :描画座標Ｘ(float)
//｜引数  :描画座標Ｙ(float)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void Panel::Draw(float x, float y)
{
	m_sprite->Begin();

	m_sprite->Draw(m_texture.Get(), XMFLOAT2(x, y), nullptr, Colors::White);

	m_sprite->End();
}


