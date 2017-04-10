//************************************************/
//* @file  :Pamel.cpp
//* @brief :道パネルのソースファイル
//* @date  :2017/04/09
//* @author:S.Katou
//************************************************/
#include "Panel.h"

#include <random>
#include <SimpleMath.h>

using namespace DirectX;

//１辺の大きさ
const int Panel::SIZE         = 128;
const int Panel::MAX_PASS_NUM = 7;

//パス一覧                                 上　　下　　左　 右
const PanelPass Panel::passTopRight    = { true, false,false,true,  L"Resources\\Road3.png" };
const PanelPass Panel::passTopLeft     = { true, false,true, false, L"Resources\\Road4.png" };
const PanelPass Panel::passBottomRight = { false,true, false,true,  L"Resources\\Road5.png" };
const PanelPass Panel::passBottomLeft  = { false,true, true, false, L"Resources\\Road6.png" };
const PanelPass Panel::passRightLeft   = { false,false,true, true,  L"Resources\\Road1.png" };
const PanelPass Panel::passTopBottom   = { true, true, false,false, L"Resources\\Road2.png" };
const PanelPass Panel::passNone        = { false,false,false,false, L"Resources\\Road7.png" };

//＋ーーーーーーーーーーーーーー＋
//｜機能  :パネルのパスをランダムに返す
//｜      :ランダム生成用
//｜引数  :なし(void)
//｜戻り値:パス構造体(PanelPass)
//＋ーーーーーーーーーーーーーー＋
PanelPass Panel::GetRandomPass()
{
	//乱数生成用
	std::random_device random;
	std::mt19937 mt(random());

	//乱数の値を制限
	std::uniform_int_distribution<int> num(0, 100);
	
	PanelPass pass;

	int result = num(mt);

	if		(result < 10) { pass = passRightLeft;	}
	else if (result < 26) { pass = passTopRight;	}
	else if (result < 42) { pass = passTopLeft;		}
	else if (result < 58) { pass = passBottomRight;	}
	else if (result < 74) { pass = passBottomLeft;	}
	else if (result < 90) { pass = passTopBottom;	}
	else				  { pass = passNone;		}

	return pass;
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :コンストラクタ
//｜引数  :デバイス				(ComPtr<ID3D11Device>)
//｜引数  :デバイスコンテキスト	(ComPtr<ID3D11DeviceContext>)
//｜引数  :ファイルパス			(wchar_t*)
//＋ーーーーーーーーーーーーーー＋
Panel::Panel(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext
	, const PanelPass& pass)
{
	//スプライトバッチの設定
	m_sprite = std::make_unique<SpriteBatch>((deviceContext.Get()));

	//画像の読み込み
	CreateWICTextureFromFile(device.Get(), pass.path, nullptr, m_texture.ReleaseAndGetAddressOf());


	//道がある方向を設定
	m_canPass[TOP]    = pass.top;
	m_canPass[BOTTOM] = pass.bottom;
	m_canPass[LEFT]   = pass.left;
	m_canPass[RIGHT]  = pass.right;

	//隣接するパネル 上下左右
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

	//描画　128.0fは元画像のサイズです
	m_sprite->Draw(m_texture.Get(), XMFLOAT2(x, y), nullptr, Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), SIZE / 128.0f);

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
	if (m_canPass[direction] == false)
	{
		return false;
	}


	//指定方向にパネルが無ければ進めない
	if (m_linkPanel[direction] == nullptr)
	{
		return true;
	}

	//隣接するパネルに道が無ければ進めない
	if (CanPassLinkPanel(direction) == false)
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
		break;
	case BOTTOM:
		canPass = m_linkPanel[direction]->m_canPass[TOP];
		break;

	case LEFT:
		canPass = m_linkPanel[direction]->m_canPass[RIGHT];
		break;
	case RIGHT:
		canPass = m_linkPanel[direction]->m_canPass[LEFT];
		break;

	default:
		break;
	}

	return canPass;
}


