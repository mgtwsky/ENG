#pragma once

#include "Wall.h"
#include "Bullet.h"
#include "Player.h"
#include "GameConstants.h"
#include "Wind.h"

class GameState {
public:
	GameState();
	~GameState();
	void CreateBullet(Vector3& const position, Vector3& const direction = Vector3{}, BallisticsType type = BallisticsType::NORMAL);
	void UpdateBullets(float const & elapsed);
	void DestroyDeadBullets();
	void SetBulletsSize(float const & size);
	GameConstants				constants;
	Player						player;
	BallisticsType				creation_bullet_type;
	std::vector<Wall>			walls;
	std::vector<Bullet>			bullets;
	std::vector<Wind>           winds;
private:
	void UpdateBulletNormal(Bullet& bullet, const float & elapsed);
	void CheckWindAffection(Bullet & bullet, const float & elapsed);
	void UpdateBulletSimple(Bullet& bullet, const float & elapsed);
	void UpdateBulletAdvanced(Bullet& bullet, const float & elapsed);
	void UpdateBulletRealistic(Bullet& bullet, const float & elapsed);
	bool CheckBulletCollisionGaps(const Bullet& bullet, const Vector3& before_move, const Vector3& after_move) const;
	void CheckBulletsCollisions();
	bool CheckWallCollision(Hitbox const & hitbox) const;
	void CreateWinds();
	float						bullet_size;
	float						bullet_hitbox_size;
};

