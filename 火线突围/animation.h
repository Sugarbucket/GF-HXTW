#pragma once
#include "atlas.h"

class Animation
{
public:
	Animation(Atlas* atlas, int interval);
	~Animation() = default;

	void AniPlay(int x, int y, int delta);

private:
	int timer = 0;
	int interval_ms = 0;
	int idx_frame = 0;
	Atlas* anim_atlas;
};
