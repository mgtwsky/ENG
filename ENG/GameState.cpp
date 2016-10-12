#include "pch.h"
#include "GameState.h"

GameState::GameState() : player{}
{
	walls		= std::vector<Wall>();
	bullets		= std::vector<Bullet>(128);
}


GameState::~GameState()
{
}

void GameState::CreateBullet(Vector3 & const position, Vector3 & const direction, BallisticsType type)
{
	Bullet bullet{};
	bullet.SetPosition(position);
	bullet.direction = direction;
	bullet.ballistics_type = type;
	bullets.emplace_back(bullet);
}

void GameState::UpdateBullets(float const & elapsed)
{
	for (auto& bullet : bullets) {
		switch (bullet.ballistics_type)
		{
		case BallisticsType::NORMAL:
			UpdateBulletNormal(bullet, elapsed);
			break;
		case BallisticsType::SIMPLE:
			UpdateBulletSimple(bullet, elapsed);
			break;
		case BallisticsType::ADVANCED:
			UpdateBulletAdvanced(bullet, elapsed);
			break;
		case BallisticsType::REALISTIC:
			UpdateBulletRealistic(bullet, elapsed);
			break;
		}
	}
	CheckBulletCollisions();
}

void GameState::DestroyDeadBullets()
{
	bullets.erase(
		remove_if(
			bullets.begin(), 
			bullets.end(), 
			[](const Bullet& o) {return o.alive.Passed() || !o.is_alive; }), 
		bullets.end());
}

void GameState::UpdateBulletNormal(Bullet& bullet, const float & elapsed)
{
	Vector3 move = bullet.direction;
	move *= constants.bullet_normal_speed;					// Make directional vector a proper length.
	move *= elapsed;										// Multiply by time delta ofc.
	bullet.direction += constants.gravity_vec * elapsed;	// Apply gravity.
	bullet.SetPosition(bullet.GetPosition() + move);
	bullet.Update(elapsed);
}

void GameState::UpdateBulletSimple(Bullet & bullet, const float & elapsed)
{
	//TODO implement simple.
}

void GameState::UpdateBulletAdvanced(Bullet & bullet, const float & elapsed)
{
	//TODO implement advanced.
}

void GameState::UpdateBulletRealistic(Bullet & bullet, const float & elapsed)
{
	//TODO implement realistic.
}

void GameState::CheckBulletCollisions()
{
	for (auto& bullet : bullets) {
		for (auto& wall : walls) {
			if (bullet.hitbox.Collides(wall.hitbox))
				bullet.is_alive = false;
		}
	}
}
