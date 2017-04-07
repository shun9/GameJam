//************************************************/
//* @file  :Pamel.cpp
//* @brief :道パネルのソースファイル
//* @date  :2017/04/07
//* @author:S.Katou
//************************************************/
#include "Panel.h"

using namespace DirectX;

//１辺の大きさ
int Panel::SIZE = 64;

//＋ーーーーーーーーーーーーーー＋
//｜機能  :コンストラクタ
//｜引数  :デバイス				(ComPtr<ID3D11Device>)
//｜引数  :デバイスコンテキスト	(ComPtr<ID3D11DeviceContext>)
//｜引数  :ファイルパス			(wchar_t*)
//＋ーーーーーーーーーーーーーー＋
Panel::Panel(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext
	, const wchar_t* path
	, const PanelPass& pass)
{
	//スプライトバッチの設定
	m_sprite = std::make_unique<SpriteBatch>((deviceContext.Get()));

	//画像の読み込み
	CreateWICTextureFromFile(device.Get(), path, nullptr, m_texture.ReleaseAndGetAddressOf());


	//道がある方向を設定
	m_canPass[TOP] = pass.top;
	m_canPass[BOTTOM] = pass.bottom;
	m_canPass[LEFT] = pass.left;
	m_canPass[RIGHT] = pass.right;

	//隣接するパネル
	for (int i = 0; i < 4; i++)
	{
		m_linkPanel[i] = nullptr;
	}
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :デストラクタ
//＋ーーーーーーーーーーーーーー＋
Panel::~Panel()
{
	m_sprite.reset();
	m_texture.Reset();
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


//＋ーーーーーーーーーーーーーー＋
//｜機能  :隣接するパネルを登録する
//｜引数  :登録するパネル(Panel*)
//｜引数  :隣接する方向　(int)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void Panel::Register(Panel* panel, int direction)
{
	m_linkPanel[direction] = panel;
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :指定された方向に進めるかどうか
//｜引数  :方向(int)
//｜戻り値:進めたらtrue(bool)
//＋ーーーーーーーーーーーーーー＋
bool Panel::CanPass(int direction)
{
	//自身に道が無ければ進めない
	if (!m_canPass[direction])
	{
		return false;
	}


	//指定方向にパネルが無ければ進めない
	if (m_linkPanel[direction] == nullptr)
	{
		return false;
	}

	//隣接するパネルに道が無ければ進めない
	if (!CanPassLinkPanel(direction))
	{
		return false;
	}

	//問題なければ進める
	return true;
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :指定された方向のパネルに進めるかどうか
//｜引数  :方向(int)
//｜戻り値:進めたらtrue(bool)
//＋ーーーーーーーーーーーーーー＋
bool Panel::CanPassLinkPanel(int direction)
{

	bool canPass = false;

	//進行方向と対になる位置の道を調べる
	switch (direction)
	{
	case TOP:
		canPass = m_linkPanel[direction]->m_canPass[BOTTOM];
	case BOTTOM:
		canPass = m_linkPanel[direction]->m_canPass[TOP];

	case LEFT:
		canPass = m_linkPanel[direction]->m_canPass[LEFT];
	case RIGHT:
		canPass = m_linkPanel[direction]->m_canPass[RIGHT];

	default:
		break;
	}

	return canPass;
}


