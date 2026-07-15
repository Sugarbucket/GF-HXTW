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
	region_btn_start_game.top = 380;
	region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;

	region_btn_settings.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_settings.right = region_btn_settings.left + BUTTON_WIDTH;
	region_btn_settings.top = 490;
	region_btn_settings.bottom = region_btn_settings.top + BUTTON_HEIGHT;

	region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
	region_btn_quit_game.top = 600;
	region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;

	region_btn_back.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_back.right = region_btn_back.left + BUTTON_WIDTH;
	region_btn_back.top = 560;
	region_btn_back.bottom = region_btn_back.top + BUTTON_HEIGHT;

	player = new Player();

	btn_start_game = new StartGameButton(region_btn_start_game, _T("img/button/start_1.png"), _T("img/button/start_3.png"), _T("img/button/start_2.png"));
	btn_quit_game = new QuitGameButton(region_btn_quit_game, _T("img/button/exit_1.png"), _T("img/button/exit_3.png"), _T("img/button/exit_2.png"));
	btn_settings = new SettingsButton(region_btn_settings, _T("img/button/setting_1.png"), _T("img/button/setting_3.png"), _T("img/button/setting_2.png"));
	btn_back = new BackButton(region_btn_back, _T("img/button/back_1.png"), _T("img/button/back_3.png"), _T("img/button/back_2.png"));

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
	delete btn_settings;
	delete btn_back;
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
		else if (is_in_settings)
		{
			btn_back->ProcessEvent(msg);
			if (msg.message == WM_KEYDOWN)
			{
				if (msg.vkcode == VK_LEFT || msg.vkcode == 'A')
				{
					bgm_volume = max(0, bgm_volume - 20);
					TCHAR cmd[64];
					_stprintf_s(cmd, _T("setaudio bgm volume to %d"), bgm_volume);
					mciSendString(cmd, NULL, 0, NULL);
				}
				else if (msg.vkcode == VK_RIGHT || msg.vkcode == 'D')
				{
					bgm_volume = min(1000, bgm_volume + 20);
					TCHAR cmd[64];
					_stprintf_s(cmd, _T("setaudio bgm volume to %d"), bgm_volume);
					mciSendString(cmd, NULL, 0, NULL);
				}
			}
		}
		else
		{
			btn_start_game->ProcessEvent(msg);
			btn_quit_game->ProcessEvent(msg);
			btn_settings->ProcessEvent(msg);
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
	MessageBox(GetHWnd(), _T("Code:3"), _T("Game over!"), MB_OK);
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
	if (btn_settings) btn_settings->ResetStatus();
	is_in_settings = false;
}

void Run::DrawSettingsPage()
{
	setbkmode(TRANSPARENT);

	settextstyle(36, 0, _T("Consolas"));
	settextcolor(RGB(200, 200, 200));
	outtextxy(WINDOW_WIDTH / 2 - 250, 280, _T("Volume"));

	int bar_left = WINDOW_WIDTH / 2 - 200;
	int bar_top = 340;
	int bar_width = 400;
	int bar_height = 30;
	int fill_width = (int)(bar_width * (bgm_volume / 1000.0f));

	setfillcolor(RGB(80, 80, 80));
	solidroundrect(bar_left, bar_top, bar_left + bar_width, bar_top + bar_height, 10, 10);

	setfillcolor(RGB(0, 180, 80));
	solidroundrect(bar_left, bar_top, bar_left + fill_width, bar_top + bar_height, 10, 10);

	settextstyle(24, 0, _T("Consolas"));
	settextcolor(RGB(200, 200, 200));
	TCHAR vol_text[16];
	_stprintf_s(vol_text, _T("%d"), bgm_volume);
	RECT vol_rect = { bar_left + bar_width + 20, bar_top, bar_left + bar_width + 80, bar_top + bar_height };
	drawtext(vol_text, &vol_rect, DT_LEFT | DT_VCENTER);

	settextstyle(20, 0, _T("Consolas"));
	settextcolor(RGB(150, 150, 150));
	RECT hint_rect = { 0, 420, WINDOW_WIDTH, 460 };
	drawtext(_T("Use Left/Right or A/D to adjust volume"), &hint_rect, DT_CENTER | DT_VCENTER);

	btn_back->Draw();
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
	else if (is_in_settings)
	{
		putimage(0, 0, &img_menu);
		DrawSettingsPage();
	}
	else
	{
		putimage(0, 0, &img_menu);
		if (btn_start_game) btn_start_game->Draw();
		if (btn_quit_game) btn_quit_game->Draw();
		if (btn_settings) btn_settings->Draw();
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
