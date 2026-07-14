#include "player.h"

Player::Player()
{
	anim_left = new Animation(atlas_player_left, 100);
	anim_right = new Animation(atlas_player_right, 100);
}

Player::~Player()
{
	delete anim_left;
	delete anim_right;
}

void Player::processmessage(const ExMessage& msg)
{
	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case 87:
			is_move_up = true;
			break;
		case 83:
			is_move_down = true;
			break;
		case 65:
			is_move_left = true;
			break;
		case 68:
			is_move_right = true;
			break;
		}
		break;
	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case 87:
			is_move_up = false;
			break;
		case 83:
			is_move_down = false;
			break;
		case 65:
			is_move_left = false;
			break;
		case 68:
			is_move_right = false;
			break;
		}
		break;
	}
}

void Player::Move()
{
	int dir_x = is_move_right - is_move_left;
	int dir_y = is_move_down - is_move_up;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	if (len_dir != 0)
	{
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;
		player_pos.x += (int)(normalized_x * PLAYER_SPEED);
		player_pos.y += (int)(normalized_y * PLAYER_SPEED);
	}

	if (player_pos.x < 0) player_pos.x = 0;
	if (player_pos.y < 0) player_pos.y = 0;
	if (player_pos.x + PLAYER_WIDTH > WINDOW_WIDTH) player_pos.x = WINDOW_WIDTH - PLAYER_WIDTH;
	if (player_pos.y + PLAYER_HEIGHT > WINDOW_HEIGHT) player_pos.y = WINDOW_HEIGHT - PLAYER_HEIGHT;
}

void Player::ResetPosition()
{
	player_pos = { 500, 500 };
}

void Player::Draw(int delta)
{
	static bool facing_left = false;
	int dir_x = is_move_right - is_move_left;
	if (dir_x < 0)
		facing_left = true;
	else if (dir_x > 0)
		facing_left = false;

	if (facing_left)
	{
		anim_left->AniPlay(player_pos.x, player_pos.y, delta);
	}
	else
	{
		anim_right->AniPlay(player_pos.x, player_pos.y, delta);
	}
}

const POINT& Player::getPosition() const
{
	return player_pos;
}

void Player::Shoot(POINT target_pos)
{
	DWORD current_time = GetTickCount();
	if (current_time - last_shoot_time < SHOOT_COOLDOWN) return;

	POINT center =
	{
		player_pos.x + PLAYER_WIDTH / 2,
		player_pos.y + PLAYER_HEIGHT / 2
	};
	sbullets.push_back(new SBullet(center, target_pos));
	last_shoot_time = current_time;
}

void Player::UpdateSBullet()
{
	auto it = sbullets.begin();
	while (it != sbullets.end())
	{
		(*it)->Move();
		if (!(*it)->IsActive())
		{
			delete* it;
			it = sbullets.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Player::DrawSBullet() const
{
	for (const auto& bullet : sbullets)
	{
		bullet->Draw();
	}
}

void Player::ClearBullets()
{
	for (auto bullet : sbullets)
	{
		delete bullet;
	}
	sbullets.clear();
}

const std::vector<SBullet*>& Player::GetBullets() const
{
	return sbullets;
}
