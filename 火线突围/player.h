#pragma once
#include "animation.h"
#include "sbullet.h"

class Player
{
public:
	static const int FRAME_WIDTH = 80;
	static const int FRAME_HEIGHT = 80;

public:
	Player();
	~Player();

	void processmessage(const ExMessage& msg);
	void Move();
	void ResetPosition();
	void Draw(int delta);
	const POINT& getPosition() const;
	void Shoot(POINT target_pos);
	void UpdateSBullet();
	void DrawSBullet() const;
	void ClearBullets();
	const std::vector<SBullet*>& GetBullets() const;

private:
	const int PLAYER_WIDTH = 80;
	const int PLAYER_HEIGHT = 80;
	const int PLAYER_SPEED = 4;

	Animation* anim_left;
	Animation* anim_right;
	POINT player_pos = { 500, 500 };
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;

	std::vector<SBullet*> sbullets;
	const int SHOOT_COOLDOWN = 500;
	DWORD last_shoot_time = 0;
};
