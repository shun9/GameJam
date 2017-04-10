//************************************************/
//* @file  :GamePlay.cpp
//* @brief :プレイシーン
//* @date  :2017/04/10
//* @author:S.Katou
//************************************************/

#include "..\..\pch.h"
#include "..\..\Game.h"
#include "GamePlay.h"

#include "..\8\ADX2Le.h"
#include "..\..\Sounds\GamePlaySounds.h"


using namespace std;
using namespace DirectX;

const int GamePlay::MAP_Y             = 3;
const int GamePlay::MAP_X             = 8;
const int GamePlay::MAP_POS_X         = 0;
const int GamePlay::MAP_POS_Y         = 20;
const int GamePlay::MAX_OPTION        = 3;
const float GamePlay::OPTION_POS_X[3] = {158.0f,343.0f,521.0f};
const float GamePlay::OPTION_POS_Y    = 455.0f;

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
	, m_scrollSpd(0.2f)
	, m_numChoosed(-1)
	, m_score(0)
	, m_isGameOver(false)
{	

	//次のシーン
	m_next = PLAY;

	ADX2Le::LoadAcb("Sounds\\GamePlaySounds.acb", "Sounds\\GamePlaySounds.awb");
	ADX2Le::Play(CRI_GAMEPLAYSOUNDS_CONVEYOR);

	//マウスの取得
	m_mouse = MouseManager::GetInstance();


	m_sprite = unique_ptr<SpriteBatch>(new SpriteBatch(m_context.Get()));
	m_commonStates = unique_ptr<CommonStates>(new CommonStates(m_device.Get()));
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\Road7.png", nullptr, m_back.ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\Result.png", nullptr, m_result.ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\BacktoTitle.png", nullptr, m_backToTitle.ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\stage.png", nullptr, m_stage.ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\flame.png", nullptr, m_flame.ReleaseAndGetAddressOf());

	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number0.png", nullptr, m_number[0].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number1.png", nullptr, m_number[1].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number2.png", nullptr, m_number[2].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number3.png", nullptr, m_number[3].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number4.png", nullptr, m_number[4].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number5.png", nullptr, m_number[5].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number6.png", nullptr, m_number[6].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number7.png", nullptr, m_number[7].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number8.png", nullptr, m_number[8].ReleaseAndGetAddressOf());
	CreateWICTextureFromFile(m_device.Get(), L"Resources\\number9.png", nullptr, m_number[9].ReleaseAndGetAddressOf());

	//ステージの作成
	CreateStage();

	//選択肢の作成
	CreateOption();

	m_player = new Player(MAP_POS_X+Panel::SIZE+20,
						  MAP_POS_Y+Panel::SIZE+MAP_POS_Y,
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
	
	//ゲーム進行中の処理
	if (!m_isGameOver)
	{

	//マウスの座標を更新 間に挟むため半分ずらす
	m_mousePosX = (m_mouse->GetPosX() + m_scrollPos+ (Panel::SIZE / 2) - MAP_POS_X) / Panel::SIZE ;
	m_mousePosY = (m_mouse->GetPosY()-MAP_POS_Y) / Panel::SIZE;

	//ステージ移動
	UpdateStage();

	//選択肢の更新
	UpdateOption();

	//プレイヤーの更新
	UpdatePlayer();

	//ゲームオーバー判定
	m_isGameOver = IsDead();
	}

	//ゲームオーバー中の処理
	if (m_isGameOver)
	{
		m_cntTime++;
		GameOver();
	}
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :描画処理
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::Render()
{
	//描画開始
	m_sprite->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied(), m_commonStates->PointClamp());
	
	//背景描画
	m_sprite->Draw(m_back.Get(), DirectX::XMFLOAT2(0.0f, 0.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(10.0f, 3.44f));
	
	//フレーム描画
	m_sprite->Draw(m_flame.Get(), DirectX::XMFLOAT2(0.0f, 440.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));

	m_sprite->End();


	//ステージの描画
	DrawStage();

	//描画開始
	m_sprite->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied(), m_commonStates->PointClamp());
	//とげ描画
	m_sprite->Draw(m_stage.Get(), DirectX::XMFLOAT2(-12.0f, -1.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.05f, 1.1f));
	m_sprite->End();


	//選択肢の描画
	DrawOption();

	//プレイヤー描画
	m_player->Render();



	//描画開始
	m_sprite->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied(), m_commonStates->PointClamp());

	if (m_isGameOver)
	{
		//リザルト画面
		m_sprite->Draw(m_result.Get(), DirectX::XMFLOAT2(100.0f, 70.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
		m_sprite->Draw(m_backToTitle.Get(), DirectX::XMFLOAT2(180.0f, 450.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
		
		//スコア
		m_sprite->Draw(m_number[m_scoreDigit[0]].Get(), DirectX::XMFLOAT2(200.0f, 200.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(3.0f, 3.0f));
		m_sprite->Draw(m_number[m_scoreDigit[1]].Get(), DirectX::XMFLOAT2(350.0f, 200.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(3.0f, 3.0f));
		m_sprite->Draw(m_number[m_scoreDigit[2]].Get(), DirectX::XMFLOAT2(500.0f, 200.0f), nullptr, DirectX::Colors::Crimson, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(3.0f, 3.0f));
	}

	m_sprite->End();

}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :ステージの更新
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::UpdateStage()
{
	//スクロールする
	m_scrollPos += m_scrollSpd;

	//１パネル分スクロールしたら
	if (m_scrollPos >= Panel::SIZE)
	{
		//パネルをずらす
		PanelSlide();

		//スコアを加算
		m_score++;

		//位置を戻す
		m_scrollPos = 0.0f;

		//スピードアップ
		m_scrollSpd += 0.01f;
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
	DirectX::SimpleMath::Vector2 pos = m_player->getPos();
	int x = (pos.x +m_scrollPos - MAP_POS_X) / Panel::SIZE;
	int y = (pos.y - MAP_POS_Y) / Panel::SIZE;

	m_player->registerPanel(m_panel[y][x]);

	m_player->Update(m_scrollSpd);
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

	//ステージ外に出たらゲームオーバー
	if (pos.x < 0.0f
	||  pos.x > 740.0f          
	||  pos.y < 0.0f
	||  pos.y >  Panel::SIZE * MAP_Y-30.0f)
	{
		ADX2Le::LoadAcb("Sounds\\GamePlaySounds.acb", "Sounds\\GamePlaySounds.awb");
		ADX2Le::Play(CRI_GAMEPLAYSOUNDS__CUE_ID_7);
		DivideScore();
		return true;
	}

	return false;
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :ゲームオーバー判定
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::GameOver()
{
	if (m_cntTime >= 60)
	{
		if (m_mouse->IsClickedLeft())
		{
			m_next = TITLE;
		}
	}
}

//＋ーーーーーーーーーーーーーー＋
//｜機能  :スコアを桁ごとに分解する
//｜引数  :なし(void)
//｜戻り値:なし(void)
//＋ーーーーーーーーーーーーーー＋
void GamePlay::DivideScore()
{
	//1000でカンスト
	if (m_score >= 1000)
	{
		m_score = 999;
		m_scoreDigit[0] = 9;
		m_scoreDigit[1] = 9;
		m_scoreDigit[2] = 9;
		return;
	}
	
	//100の位
	int digit100 = 0;
	while (m_score >= 100)
	{
		digit100++;
		m_score -= 100;
	}
	m_scoreDigit[0] = digit100;

	//10の位
	int digit10 = 0;
	while (m_score >= 10)
	{
		digit10++;
		m_score -= 10;
	}
	m_scoreDigit[1] = digit10;

	//1の位
	int digit1 = 0;
	while (m_score >= 1)
	{
		digit1++;
		m_score -= 1;
	}

	m_scoreDigit[2] = digit1;

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
