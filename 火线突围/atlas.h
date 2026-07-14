#pragma once
#include "common.h"

class Atlas
{
public:
	Atlas(LPCTSTR path, int num);
	~Atlas();

	std::vector<IMAGE*> frame_list;
};

extern Atlas* atlas_player_left;
extern Atlas* atlas_player_right;
extern Atlas* atlas_enemy_left;
extern Atlas* atlas_enemy_right;
