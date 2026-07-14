#pragma once
#include "common.h"

class SBullet
{
	POINT pos;
	const int speed = 10;
	double dir_x, dir_y;
	bool active = true;
	mutable IMAGE img_SBullet;
public:
	SBullet(POINT start_pos, POINT target_pos);

	void Move();
	void Draw() const;

	bool IsActive() const { return active; }
	POINT GetPosition() const { return pos; }
	void Deactivate() { active = false; }
};
