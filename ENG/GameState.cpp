#include "pch.h"
#include "GameState.h"

GameState::GameState() : player{}, ballistics_type{ BallisticsType::NORMAL }
{
	walls		= std::vector<Wall>();
	bullets		= std::vector<Bullet>(128);
}


GameState::~GameState()
{
}

void GameState::CreateBullet(Vector3 & const position, Vector3 & const direction)
{
	Bullet bullet{};
	bullet.SetPosition(position);
	bullet.direction = direction;
	bullets.emplace_back(bullet);
}

void GameState::UpdateBullets(float const & elapsed)
{
	switch (ballistics_type)
	{
	case BallisticsType::NORMAL:
		UpdateBulletsNormal(elapsed);
		break;
	default:
		ErrorCheck(false, "UNKNOWN BALLISTIC MODEL SELECTED!");
		break;
	}
}

void GameState::DestroyDeadBullets()
{
	bullets.erase(
		remove_if(
			bullets.begin(), 
			bullets.end(), 
			[](const Bullet& o) {return o.alive.Passed(); }), 
		bullets.end());
}

void GameState::UpdateBulletsNormal(const float & elapsed)
{
	for (auto& bullet : bullets) {
		Vector3 move = bullet.direction;
		move *= constants.bullet_normal_speed;
		move += constants.gravity_vec;
		move *= elapsed;
		bullet.SetPosition(bullet.GetPosition() + move);
		bullet.Update(elapsed);
	}
}
