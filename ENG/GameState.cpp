#include "pch.h"
#include "GameState.h"

GameState::GameState() : player{}
{
	walls		= std::vector<Wall>();
	bullets		= std::vector<Bullet>();
}


GameState::~GameState()
{
}

void GameState::CreateBullet(Vector3 & const position, Vector3 & const direction, BallisticsType type)
{
	Bullet bullet{};
	bullet.SetPosition(position);
	bullet.hitbox.SetExtends(Vector3(0.175f));
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
}

void GameState::DestroyDeadBullets()
{
	bullets.erase(
		remove_if(
			bullets.begin(), 
			bullets.end(), 
			[](const Bullet& o) { return o.alive.Passed() || !o.is_alive; }), 
		bullets.end());
}

void GameState::UpdateBulletNormal(Bullet& bullet, float const & elapsed)
{
#pragma region Bullet movement.
	const Vector3 before_move = bullet.GetPosition();
	{
		Vector3 move = bullet.direction;
		move *= constants.bullet_normal_speed;					// Make directional vector a proper length.
		move *= elapsed;										// Multiply by time delta ofc.
		bullet.direction += constants.gravity_vec * elapsed;	// Apply gravity.
		bullet.SetPosition(bullet.GetPosition() + move);		// Actually move.
	}
	const Vector3 after_move = bullet.GetPosition();
#pragma endregion
#pragma region Collision gaps.
	const float travel_length	= (after_move - before_move).Length();		// If bullet has travelled so far that it creates gaps of collisions.
	const float hitbox_length	= bullet.hitbox.GetExtends().Length() * 2;
	const int overtravel_times	= travel_length / hitbox_length;
	if (overtravel_times > 0) {
		const Vector3 move_cut{ (after_move - before_move) / (overtravel_times + 1) };
		for (int i = 1; i <= overtravel_times; i++)
		{
			const Vector3 center{ before_move + (move_cut * i) };
			const Hitbox hitbox{ center, bullet.hitbox.GetExtends() };
			if (CheckWallCollision(hitbox)) bullet.is_alive = false;
		}
	}
#pragma endregion
#pragma region Collision check.
	if (CheckWallCollision(bullet.hitbox)) bullet.is_alive = false;
#pragma endregion
	bullet.Update(elapsed);
}

void GameState::UpdateBulletSimple(Bullet & bullet, const float & elapsed)
{
	const Vector3 before_move = bullet.GetPosition();
}

void GameState::UpdateBulletAdvanced(Bullet & bullet, const float & elapsed)
{
	//TODO implement advanced.
#pragma region Bullet movement.
	const Vector3 before_move = bullet.GetPosition();
	{
		Vector3 move = bullet.direction;
		move *= constants.bullet_normal_speed;					// Make directional vector a proper length.
		move *= elapsed;										// Multiply by time delta ofc.
		bullet.direction += constants.gravity_vec * elapsed;	// Apply gravity.
		bullet.SetPosition(bullet.GetPosition() + move);		// Actually move.
	}
	const Vector3 after_move = bullet.GetPosition();
#pragma endregion
}

void GameState::UpdateBulletRealistic(Bullet & bullet, const float & elapsed)
{
	//TODO implement realistic.
#pragma region Bullet movement.
	const Vector3 before_move = bullet.GetPosition();
	{
		Vector3 move = bullet.direction;
		move *= constants.bullet_normal_speed;					// Make directional vector a proper length.
		move *= elapsed;										// Multiply by time delta ofc.
		bullet.direction += constants.gravity_vec * elapsed;	// Apply gravity.
		bullet.SetPosition(bullet.GetPosition() + move);		// Actually move.
	}
	const Vector3 after_move = bullet.GetPosition();
#pragma endregion
}

void GameState::CheckBulletsCollisions()
{
	for (auto& bullet : bullets) {
		for (auto const & wall : walls) {
			if (bullet.hitbox.Collides(wall.hitbox)) {
				bullet.is_alive = false;
			}
		}
	}
}

bool GameState::CheckWallCollision(Hitbox const & hitbox) const
{
	for (auto const & wall : walls) {
		if (hitbox.Collides(wall.hitbox))
			return true;
	}
	return false;
}
