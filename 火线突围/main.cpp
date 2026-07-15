#include "common.h"
#include "run.h"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

const int BUTTON_WIDTH = 400;
const int BUTTON_HEIGHT = 100;

int idx_current_anim = 0;

bool is_game_started = false;
bool is_in_settings = false;
bool running = true;
int bgm_volume = 200;

Atlas* atlas_player_left;
Atlas* atlas_player_right;
Atlas* atlas_enemy_left;
Atlas* atlas_enemy_right;

int main()
{
	Run run;
	run.Start();
	return 0;
}
