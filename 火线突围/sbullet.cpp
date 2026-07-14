#include "sbullet.h"

SBullet::SBullet(POINT start_pos, POINT target_pos) : pos(start_pos)
{
	loadimage(&img_SBullet, _T("img/SBullet.png"));

	int dx = target_pos.x - start_pos.x;
	int dy = target_pos.y - start_pos.y;
	double len = sqrt(dx * dx + dy * dy);
	if (len > 0)
	{
		dir_x = dx / len;
		dir_y = dy / len;
	}
	else
	{
		dir_x = 0;
		dir_y = 0;
	}
}

void SBullet::Move()
{
	if (!active) return;

	pos.x += (int)(dir_x * speed);
	pos.y += (int)(dir_y * speed);
	if (pos.x < -50 || pos.x > 1280 + 50 || pos.y < -50 || pos.y > 720 + 50)
	{
		active = false;
	}
}

void SBullet::Draw() const
{
	if (active)
	{
		putimage_alpha(pos.x, pos.y, &img_SBullet);
	}
}
