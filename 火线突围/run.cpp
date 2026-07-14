#include "run.h"

Run::Run()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);

	atlas_player_left = new Atlas(_T("img/player/player_left_%d.png"), 14);
	atlas_player_right = new Atlas(_T("img/player/player_right_%d.png"), 14);
	atlas_enemy_left = new Atlas(_T("img/enemy/enemy_left_%d.png"), 7);
	atlas_enemy_right = new Atlas(_T("img/enemy/enemy_right_%d.png"), 7);

	mciSendString(_T("open music/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("setaudio bgm volume to 200"), NULL, 0, NULL);
	mciSendString(_T("open music/hit.mp3 alias hit"), NULL, 0, NULL);

	region_btn_start_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
	region_btn_start_game.top = 430;
	region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;

	region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
	region_btn_quit_game.top = 560;
	region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;

	player = new Player();

	btn_start_game = new StartGameButton(region_btn_start_game, _T("img/button/start_1.png"), _T("img/button/start_3.png"), _T("img/button/start_2.png"));
	btn_quit_game = new QuitGameButton(region_btn_quit_game, _T("img/button/exit_1.png"), _T("img/button/exit_3.png"), _T("img/button/exit_2.png"));

	loadimage(&img_menu, _T("img/menu.png"));
	loadimage(&img_background, _T("img/bg.png"));

	BeginBatchDraw();
}

Run::~Run()
{
	delete player;
	delete atlas_player_left;
	delete atlas_player_right;
	delete atlas_enemy_left;
	delete atlas_enemy_right;
	delete btn_start_game;
	delete btn_quit_game;
	EndBatchDraw();
}

void Run::TryGenerateEnemy()
{
	const int INTERVAL = 100;
	static int counter = 0;
	if (++counter % INTERVAL == 0)
	{
		enemy_list.push_back(new Enemy());
	}
}

void Run::DrawPlayerScore()
{
	static TCHAR text[64];
	_stprintf_s(text, _T("Score: %d"), score);
	setbkmode(TRANSPARENT);
	settextcolor(RGB(255, 85, 185));
	outtextxy(10, 10, text);
}

void Run::ProcessEvents(ExMessage& msg)
{
	while (peekmessage(&msg))
	{
		if (is_game_started)
		{
			player->processmessage(msg);
			if (msg.message == WM_LBUTTONDOWN)
			{
				player->Shoot({ msg.x, msg.y });
			}
		}
		else
		{
			btn_start_game->ProcessEvent(msg);
			btn_quit_game->ProcessEvent(msg);
		}
	}
}

void Run::UpdateGameLogic()
{
	player->Move();
	player->UpdateSBullet();
	TryGenerateEnemy();
	for (Enemy* enemy : enemy_list)
		enemy->Move(*player);

	for (Enemy* enemy : enemy_list)
	{
		if (enemy->CheckPlayerCollision(*player))
		{
			HandleGameOver();
			return;
		}
	}

	for (Enemy* enemy : enemy_list)
	{
		for (const SBullet* sbullet : player->GetBullets())
		{
			if (enemy->CheckSBulletCollision(*sbullet))
			{
				mciSendString(_T("play hit from 0"), NULL, 0, NULL);
				enemy->Hurt();
				score++;
			}
		}
	}

	for (size_t i = 0; i < enemy_list.size(); i++)
	{
		Enemy* enemy = enemy_list[i];
		if (!enemy->CheckAlive())
		{
			std::swap(enemy_list[i], enemy_list.back());
			enemy_list.pop_back();
			delete enemy;
		}
	}
}

void Run::HandleGameOver()
{
	MessageBox(GetHWnd(), _T("Code:3"), _T("游戏结束"), MB_OK);
	is_game_started = false;
	for (auto e : enemy_list)
	{
		delete e;
	}
	enemy_list.clear();
	player->ClearBullets();
	player->ResetPosition();
	score = 0;
	mciSendString(_T("stop bgm"), NULL, 0, NULL);
	if (btn_start_game) btn_start_game->ResetStatus();
	if (btn_quit_game) btn_quit_game->ResetStatus();
}

void Run::RenderGame()
{
	cleardevice();
	if (is_game_started)
	{
		putimage(0, 0, &img_background);
		player->Draw(1000 / 144);
		player->DrawSBullet();
		for (Enemy* enemy : enemy_list)
			enemy->Draw(1000 / 144);
		DrawPlayerScore();
	}
	else
	{
		putimage(0, 0, &img_menu);
		if (btn_start_game) btn_start_game->Draw();
		if (btn_quit_game) btn_quit_game->Draw();
	}
	crosshair.Draw();
	FlushBatchDraw();
}

void Run::Start()
{
	ExMessage msg;

	while (running)
	{
		DWORD startTime = GetTickCount();
		crosshair.Update();

		ProcessEvents(msg);

		if (is_game_started)
		{
			UpdateGameLogic();
		}

		RenderGame();

		DWORD endTime = GetTickCount();
		DWORD delta_time = endTime - startTime;
		if (delta_time < 1000 / 144)
		{
			Sleep(1000 / 144 - delta_time);
		}
	}
}
