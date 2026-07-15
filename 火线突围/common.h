#pragma once

#include<graphics.h>
#include<string>
#include<vector>
#include<cmath>

#pragma comment(lib,"MSIMG32.LIB")
#pragma comment(lib, "winmm.lib")

#define PI 3.14159

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern const int BUTTON_WIDTH;
extern const int BUTTON_HEIGHT;
extern int idx_current_anim;
extern bool is_game_started;
extern bool is_in_settings;
extern bool running;
extern int bgm_volume;

inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
