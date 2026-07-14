#pragma once
#include "animation.h"
#include "player.h"
#include "sbullet.h"

class Enemy
{
public:
	static const int FRAME_WIDTH = 80;
	static const int FRAME_HEIGHT = 100;

public:
	Enemy();
	~Enemy();

	bool CheckPlayerCollision(const Player& player);
	bool CheckSBulletCollision(const SBullet& sbullet);
	void Move(const Player& player);
	void Draw(int delta);
	void Hurt();
	bool CheckAlive() const;
	const POINT& getPosition() const;

private:
	Animation* anim_left;
	Animation* anim_right;
	POINT position = { 0, 0 };
	bool facing_left = false;
	bool alive = true;

	const int ENEMY_WIDTH = 80;
	const int ENEMY_HEIGHT = 100;
	const int ENEMY_SPEED = 3;
};
