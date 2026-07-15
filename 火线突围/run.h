#pragma once
#include "common.h"
#include "atlas.h"
#include "animation.h"
#include "sbullet.h"
#include "player.h"
#include "enemy.h"
#include "button.h"
#include "crosshair.h"

class Run
{
public:
	Run();
	~Run();

	void Start();

private:
	void TryGenerateEnemy();
	void DrawPlayerScore();
	void DrawSettingsPage();
	void ProcessEvents(ExMessage& msg);
	void UpdateGameLogic();
	void RenderGame();
	void HandleGameOver();

	int score = 0;
	Player* player = nullptr;
	IMAGE img_background;
	IMAGE img_menu;
	Crosshair crosshair;
	std::vector<Enemy*> enemy_list;

	RECT region_btn_start_game;
	RECT region_btn_quit_game;
	RECT region_btn_settings;
	RECT region_btn_back;
	StartGameButton* btn_start_game = nullptr;
	QuitGameButton* btn_quit_game = nullptr;
	SettingsButton* btn_settings = nullptr;
	BackButton* btn_back = nullptr;
};
