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
			// TODO invoke simple ballistics update.
			break;
		case BallisticsType::ADVANCED:
			// TODO invoke advanced ballistics update.
			break;
		case BallisticsType::REALISTIC:
			// TODO invoke realistic ballistics update.
			break;
		}
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

void GameState::UpdateBulletNormal(Bullet& bullet, const float & elapsed)
{
	Vector3 move = bullet.direction;
	move *= constants.bullet_normal_speed;					// Make directional vector a proper length.
	move *= elapsed;										// Multiply by time delta ofc.
	bullet.direction += constants.gravity_vec * elapsed;	// Apply gravity.
	bullet.SetPosition(bullet.GetPosition() + move);
	bullet.Update(elapsed);
}
