//--------------------------------------------------------------------------------------
// File: GamePlay.cpp
// Date: 2017.04.07
// Author: Syuto Yamada
//--------------------------------------------------------------------------------------

#include "..\..\pch.h"
#include "..\..\Game.h"
#include "GamePlay.h"

#include "..\8\ADX2Le.h"
#include "..\..\Sounds\GamePlaySounds.h"


using namespace std;

const int GamePlay::MAP_Y             = 3;
const int GamePlay::MAP_X             = 8;
const int GamePlay::MAP_POS_X         = 0;
const int GamePlay::MAP_POS_Y         = 20;
const int GamePlay::MAX_OPTION        = 3;
const float GamePlay::OPTION_POS_X[3] = {200.0f,350.0f,500.0f};
const float GamePlay::OPTION_POS_Y    = 450.0f;

//＋ーーーーーーーーーーーーーー＋
//｜機能  :コンストラクタ
//｜引数  :デバイス		(Microsoft::WRL::ComPtr<ID3D11Device>)
//｜引数  :コンテキスト (Microsoft::WRL::ComPtr<ID3D11DeviceContext>)
//＋ーーーーーーーーーーーーーー＋
GamePlay::GamePlay(Microsoft::WRL::ComPtr<ID3D11Device> device
	, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	: m_device(device)
	, m_context(context)
	, m_cntTime(0)
	, m_scrollPos(0)
	, m_numChoosed(-1)
	, m_score(0)
	, m_isGameOver(false)
{	
	//サウンドの初期化
	ADX2Le::LoadAcb("Sounds\\GamePlaySounds.acb", "Sounds\\GamePlaySounds.awb");
	ADX2Le::Play(CRI_GAMEPLAYSOUNDS__CUE_ID_1);

	m_spriteBatch = std::make_unique<SpriteBatch>(context.Get());
	CreateWICTextureFromFile(device.Get(), L"Resources\\title.png", nullptr, m_background.ReleaseAndGetAddressOf());

	//次のシーン
	m_next = PLAY;

	//マウスの取得
	m_mouse = MouseManager::GetInstance();

	//ステージの作成
	CreateStage();

	//選択肢の作成
	CreateOption();

	m_player = new Player(MAP_POS_X+Panel::SIZE,
						  MAP_POS_Y+Panel::SIZE,
						  m_device,m_context);
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :デストラクタ
//＋ーーーーーーーーーーーーーー＋
GamePlay::~GamePlay()
{
	//ステージ削除
	DeleteStage();

	//選択肢削除
	DeleteOption();
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :更新処理
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::Update()
{
	//マウス更新
	m_mouse->Update();

	//マウスの座標を更新 間に挟むため半分ずらす
	m_mousePosX = (m_mouse->GetPosX() + m_scrollPos+ (Panel::SIZE / 2) - MAP_POS_X) / Panel::SIZE ;
	m_mousePosY = (m_mouse->GetPosY()-MAP_POS_Y) / Panel::SIZE;

	//ステージ移動
	UpdateStage();

	if (m_isGameOver)
	{
		GameOver();
		return;
	}
	//選択肢の更新
	UpdateOption();

	//プレイヤーの更新
	UpdatePlayer();

}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :描画処理
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::Render()
{
	//ステージの描画
	DrawStage();

	//選択肢の描画
	DrawOption();

	//プレイヤー描画
	m_player->Render();

	if (m_isGameOver)
	{
		m_spriteBatch->Begin();
		m_spriteBatch->Draw(m_button.Get(), Vector2(m_fullscreenRect.right / 4.0f, m_fullscreenRect.bottom / 1.5f),
			nullptr, Colors::White, 0.f, m_buttonorigin);
		m_spriteBatch->End();
	}
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :ステージの更新
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::UpdateStage()
{
	//スクロールする
	m_scrollPos += 0.1f;

	//１パネル分スクロールしたら
	if (m_scrollPos >= Panel::SIZE)
	{
		//パネルをずらす
		PanelSlide();

		//スコアを加算
		m_score++;

		//位置を戻す
		m_scrollPos = 0.0f;
	}
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :選択肢の更新
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::UpdateOption()
{
	if (m_mouse->IsClickedLeft())
	{
		if (m_numChoosed <= -1)
		{
			//選択肢をとる
			m_numChoosed = ChoosedOption();
		}
	}
	else
	{
		//選択肢を選んでいる場合
		if (m_numChoosed >= 0)
		{
			FitOption();
		}

		//選択肢をリセット
		m_numChoosed = -1;
	}
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :プレイヤーの更新
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::UpdatePlayer()
{
	m_player->Update();

	DirectX::SimpleMath::Vector2 pos = m_player->getPos();
	int x = (pos.x + m_scrollPos - MAP_POS_X) / Panel::SIZE;
	int y = (pos.y - MAP_POS_Y) / Panel::SIZE;

	m_player->registerPanel(m_panel[y][x]);
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :パネルをセットする
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::FitOption()
{
	//ステージ外ならば抜ける
	if (m_mousePosX < 0 || m_mousePosX >= MAP_X
	||  m_mousePosY < 0 || m_mousePosY >= MAP_Y)
	{
		return;
	}

	//最後尾を削除
	delete m_panel[m_mousePosY][MAP_X-1];

	//パネルをずらす
	int i = MAP_X - 1;
	while (i != m_mousePosX)
	{
		m_panel[m_mousePosY][i] = m_panel[m_mousePosY][i - 1];
		i--;
	}
	//パネルをはめる
	m_panel[m_mousePosY][m_mousePosX] = m_option[m_numChoosed].panel;

	//効果音再生
	ADX2Le::Play(CRI_GAMEPLAYSOUNDS__CUE_ID_0);

	//新しい選択肢を作成
	m_option[m_numChoosed].panel = new Panel(m_device, m_context, Panel::GetRandomPass());

	LinkPanel();
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :パネルをずらす
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::PanelSlide()
{
	for (int i = 0; i < MAP_Y; i++)
	{
		//端のパネルを消す
		delete m_panel[i][0];

		for (int j = 0; j < MAP_X-1; j++)
		{
			//パネルをずらす
			m_panel[i][j] = m_panel[i][j + 1];
		}
		
		//最後尾に新しいパネルを作成
		m_panel[i][MAP_X - 1] = new Panel(m_device, m_context, Panel::GetRandomPass());
	}

	LinkPanel();
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :ゲームオーバー判定
//｜引数  :なし(void)
//｜戻り値:ゲームオーバーでtrue(bool)
//＋ーーーーーーーーーーーーーー＋
bool GamePlay::IsDead()
{
	DirectX::SimpleMath::Vector2 pos = m_player->getPos();

	if (pos.x < -Panel::SIZE / 2)
	{

	}
	return 0;
}

void GamePlay::GameOver()
{
	if (m_mouse->IsClickedLeft())
	{
		m_next = TITLE;
	}
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :マウスと接している選択肢を返す
//｜引数  :なし(void)
//｜戻り値:選択肢の番号(int)
//＋ーーーーーーーーーーーーーー＋
int GamePlay::ChoosedOption()
{
	//マウスの座標
	int mouseX = m_mouse->GetPosX();
	int mouseY = m_mouse->GetPosY();

	//選択肢番号
	int option = -1;

	for (int i = 0; i < MAX_OPTION; i++)
	{
		//マウスが選択肢に触れていた場合　抜ける
		if (m_option[i].posX               <= mouseX 
		&&  m_option[i].posX + Panel::SIZE >= mouseX
		&&  m_option[i].posY			   <= mouseY
		&&  m_option[i].posY + Panel::SIZE >= mouseY)
		{
			option = i;
			break;
		}
	}

	return option;
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :ステージのパネルを接続する
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::LinkPanel()
{
	//パネルを接続する
	for (int i = 0; i < MAP_Y; i++)
	{
		for (int j = 0; j < MAP_X; j++)
		{
			//上のパネルを接続
			if (i - 1 >= 0)
			{
				m_panel[i][j]->Register(m_panel[i - 1][j], TOP);
			}

			//下のパネルを接続
			if (i + 1 < MAP_Y)
			{
				m_panel[i][j]->Register(m_panel[i + 1][j], BOTTOM);
			}

			//左のパネルを接続
			if (j - 1 >= 0)
			{
				m_panel[i][j]->Register(m_panel[i][j-1], LEFT);
			}

			//右のパネルを接続
			if (j + 1 < MAP_X)
			{
				m_panel[i][j]->Register(m_panel[i][j+1], RIGHT);
			}
		}
	}
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :ステージの描画
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::DrawStage()
{
	//ステージの描画
	for (int i = 0; i < MAP_Y; i++)
	{
		for (int j = 0; j < MAP_X; j++)
		{
			m_panel[i][j]->Draw(j*Panel::SIZE + MAP_POS_X - m_scrollPos, i*Panel::SIZE + MAP_POS_Y);
		}
	}
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :選択肢の描画
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::DrawOption()
{
	//選択肢の描画
	for (int i = 0; i < MAX_OPTION; i++)
	{
		if (m_numChoosed == i)
		{
			m_option[i].panel->Draw(m_mouse->GetPosX() - (Panel::SIZE / 2.0f),
				m_mouse->GetPosY() - (Panel::SIZE / 2.0f));
		}
		else
		{
			m_option[i].panel->Draw(m_option[i].posX, m_option[i].posY);
		}
	}
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :ステージを作成する
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::CreateStage()
{
	// ステージを２次配列で動的確保
	m_panel = new Panel**[MAP_Y];
	for (int i = 0; i < MAP_Y; i++)
	{
		m_panel[i] = new Panel*[MAP_X];
	}

	//ステージにパネルを設定
	for (int i = 0; i < MAP_Y; i++)
	{
		for (int j = 0; j < MAP_X; j++)
		{
			m_panel[i][j] = new Panel(m_device, m_context, Panel::GetRandomPass());
		}
	}

	//パネルを接続する
	LinkPanel();
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :選択肢を作成する
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::CreateOption()
{
	m_option = new OPTION[MAX_OPTION];

	for (int i = 0; i < MAX_OPTION; i++)
	{
		m_option[i].panel = new Panel(m_device, m_context, Panel::GetRandomPass());
		m_option[i].posX = OPTION_POS_X[i];
		m_option[i].posY = OPTION_POS_Y;
	}
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :選択肢を削除する
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::DeleteStage()
{
	//パネルの削除
	for (int i = 0; i < MAP_Y; i++)
	{
		for (int j = 0; j < MAP_X; j++)
		{
			delete m_panel[i][j];
		}
	}

	for (int i = 0; i < MAP_Y; i++)
	{
		delete m_panel[i];
	}

	delete m_panel;
}


//＋ーーーーーーーーーーーーーー＋
//｜機能  :選択肢を削除する
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::DeleteOption()
{
	for (int i = 0; i < MAX_OPTION; i++)
	{
		delete m_option[i].panel;
	}
	delete[] m_option;
}
