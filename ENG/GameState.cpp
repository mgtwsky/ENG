#include "pch.h"
#include "GameState.h"

GameState::GameState() : player{} {
	walls = std::vector<Wall>();
	bullets = std::vector<Bullet>();
	winds = std::vector<Wind>();
	CreateWinds();
	creation_bullet_type = BallisticsType::NORMAL;
	SetBulletsSize(0.5f);
}

GameState::~GameState() {}

void GameState::CreateBullet(Vector3 & const position, Vector3 & const direction, BallisticsType type) {
	Bullet bullet{};
	bullet.SetPosition(position);
	bullet.SetBulletSize(bullet_size);
	bullet.hitbox.SetExtends(Vector3(bullet_hitbox_size));
	bullet.direction = direction;
	bullet.ballistics_type = type;
	bullets.emplace_back(bullet);
}

void GameState::UpdateBullets(float const & elapsed) {
	for (auto& bullet : bullets) {
		switch (bullet.ballistics_type) {
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

void GameState::DestroyDeadBullets() {
	bullets.erase(
		remove_if(
			bullets.begin(),
			bullets.end(),
			[](const Bullet& o) { return o.alive.Passed() || !o.is_alive; }),
		bullets.end());
}

void GameState::SetBulletsSize(float const & size) {
	bullet_size = size;
	bullet_hitbox_size = bullet_size * 0.75;
}

void GameState::UpdateBulletNormal(Bullet& bullet, float const & elapsed) {
	const Vector3 before_move = bullet.GetPosition();
	{
		Vector3 move = bullet.direction;
		move *= constants.bullet_normal_speed;					// Make directional vector a proper length.
		move *= elapsed;										// Multiply by time delta ofc.
		bullet.direction += constants.gravity_vec * elapsed;	// Apply gravity.
		bullet.SetPosition(bullet.GetPosition() + move);		// Actually move.
	}
	const Vector3 after_move = bullet.GetPosition();

	if (CheckBulletCollisionGaps(bullet, before_move, after_move)) bullet.is_alive = false;
	if (CheckWallCollision(bullet.hitbox)) bullet.is_alive = false;

	bullet.Update(elapsed);
}

void GameState::CheckWindAffection(Bullet & bullet, const float & elapsed) {
	for (auto const & wind : winds) {
		if (wind.Contains(bullet.hitbox)) wind.Affect(bullet, elapsed);
	}
}

void GameState::UpdateBulletSimple(Bullet & bullet, const float & elapsed) {
	const Vector3 before_move = bullet.GetPosition();

	bullet.Update(elapsed);
}

void GameState::UpdateBulletAdvanced(Bullet & bullet, const float & elapsed) {
	const Vector3 before_move = bullet.GetPosition();
	{
		Vector3 move = bullet.direction;
		move *= constants.bullet_normal_speed;					// Make directional vector a proper length.
		move *= elapsed;										// Multiply by time delta ofc.
		bullet.direction += constants.gravity_vec * elapsed;	// Apply gravity.
		bullet.SetPosition(bullet.GetPosition() + move);		// Actually move.
	}
	const Vector3 after_move = bullet.GetPosition();

	CheckWindAffection(bullet, elapsed);
	if (CheckBulletCollisionGaps(bullet, before_move, after_move)) bullet.is_alive = false;
	if (CheckWallCollision(bullet.hitbox)) bullet.is_alive = false;

	bullet.Update(elapsed);
}

void GameState::UpdateBulletRealistic(Bullet & bullet, const float & elapsed) {
	//TODO implement realistic.
	const Vector3 before_move = bullet.GetPosition();
	{
		Vector3 move = bullet.direction;
		move *= constants.bullet_normal_speed;					// Make directional vector a proper length.
		move *= elapsed;										// Multiply by time delta ofc.
		bullet.direction += constants.gravity_vec * elapsed;	// Apply gravity.
		bullet.SetPosition(bullet.GetPosition() + move);		// Actually move.
	}
	const Vector3 after_move = bullet.GetPosition();

	if (CheckBulletCollisionGaps(bullet, before_move, after_move)) bullet.is_alive = false;
	if (CheckWallCollision(bullet.hitbox)) bullet.is_alive = false;

	bullet.Update(elapsed);
}

bool GameState::CheckBulletCollisionGaps(const Bullet & bullet, const Vector3 & before_move, const Vector3 & after_move) const {
	const float travel_length = (after_move - before_move).Length();		// If bullet has travelled so far that it creates gaps of collisions.
	const float hitbox_length = bullet.hitbox.GetExtends().Length() * 2;
	const int overtravel_times = (int)travel_length / hitbox_length;;
	if (overtravel_times > 0) {
		const Vector3 move_cut{ (after_move - before_move) / (overtravel_times + 1) };
		for (int i = 1; i <= overtravel_times; i++) {
			const Vector3 center{ before_move + (move_cut * i) };
			const Hitbox hitbox{ center, bullet.hitbox.GetExtends() };
			if (CheckWallCollision(hitbox))
				return true;
		}
	}
	return false;
}

void GameState::CheckBulletsCollisions() {
	for (auto& bullet : bullets) {
		for (auto const & wall : walls) {
			if (bullet.hitbox.Collides(wall.hitbox)) {
				bullet.is_alive = false;
			}
		}
	}
}

bool GameState::CheckWallCollision(Hitbox const & hitbox) const {
	for (auto const & wall : walls) {
		if (hitbox.Collides(wall.hitbox))
			return true;
	}
	return false;
}

void GameState::CreateWinds() {
	Wind main_hall_wind{ {0.f, 20.f, 0.f}, {40.f, 40.f, 40.f}, {10.f, 0.f, 0.f} };

	winds.push_back(main_hall_wind);
}
