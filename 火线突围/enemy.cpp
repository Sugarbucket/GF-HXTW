#include "enemy.h"

Enemy::Enemy()
{
	anim_left = new Animation(atlas_enemy_left, 100);
	anim_right = new Animation(atlas_enemy_right, 100);

	enum class SpawnEdge
	{
		LEFT = 0,
		RIGHT,
		TOP,
		BOTTOM
	};

	SpawnEdge edge = (SpawnEdge)(rand() % 4);
	switch (edge)
	{
	case SpawnEdge::LEFT:
		position.x = 0;
		position.y = rand() % (WINDOW_HEIGHT - ENEMY_HEIGHT);
		break;
	case SpawnEdge::RIGHT:
		position.x = WINDOW_WIDTH - ENEMY_WIDTH;
		position.y = rand() % (WINDOW_HEIGHT - ENEMY_HEIGHT);
		break;
	case SpawnEdge::TOP:
		position.x = rand() % (WINDOW_WIDTH - ENEMY_WIDTH);
		position.y = 0;
		break;
	case SpawnEdge::BOTTOM:
		position.x = rand() % (WINDOW_WIDTH - ENEMY_WIDTH);
		position.y = WINDOW_HEIGHT - ENEMY_HEIGHT;
		break;
	default:
		break;
	}
}

Enemy::~Enemy()
{
	delete anim_left;
	delete anim_right;
}

bool Enemy::CheckPlayerCollision(const Player& player)
{
	POINT checkpoint = {
		position.x + Player::FRAME_WIDTH / 2,
		position.y + Player::FRAME_HEIGHT / 2
	};
	bool is_overlap_x = checkpoint.x >= player.getPosition().x && checkpoint.x <= player.getPosition().x + Player::FRAME_WIDTH;
	bool is_overlap_y = checkpoint.y >= player.getPosition().y && checkpoint.y <= player.getPosition().y + Player::FRAME_HEIGHT;
	return is_overlap_x && is_overlap_y;
}

bool Enemy::CheckSBulletCollision(const SBullet& sbullet)
{
	bool is_overlap_x = sbullet.GetPosition().x >= position.x && sbullet.GetPosition().x <= position.x + FRAME_WIDTH;
	bool is_overlap_y = sbullet.GetPosition().y >= position.y && sbullet.GetPosition().y <= position.y + FRAME_HEIGHT;
	return is_overlap_x && is_overlap_y;
}

void Enemy::Move(const Player& player)
{
	const POINT& player_pos = player.getPosition();
	int dir_x = player_pos.x - position.x;
	int dir_y = player_pos.y - position.y;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	if (len_dir != 0)
	{
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;
		position.x += (int)(normalized_x * ENEMY_SPEED);
		position.y += (int)(normalized_y * ENEMY_SPEED);
	}
	if (dir_x < 0)
		facing_left = true;
	else if (dir_x > 0)
		facing_left = false;
}

void Enemy::Draw(int delta)
{
	if (facing_left)
		anim_left->AniPlay(position.x, position.y, delta);
	else
		anim_right->AniPlay(position.x, position.y, delta);
}

void Enemy::Hurt()
{
	alive = false;
}

bool Enemy::CheckAlive() const
{
	return alive;
}

const POINT& Enemy::getPosition() const
{
	return position;
}
