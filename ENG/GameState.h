#pragma once

#include "Wall.h"
#include "Bullet.h"
#include "Player.h"
#include "GameConstants.h"

class GameState
{
public:
	GameState();
	~GameState();
	void CreateBullet(Vector3& const position, Vector3& const direction = Vector3{}, BallisticsType type = BallisticsType::NORMAL);
	void UpdateBullets(float const & elapsed);
	void DestroyDeadBullets();
	GameConstants				constants;
	Player						player;
	std::vector<Wall>			walls;
	std::vector<Bullet>			bullets;
private:
	void UpdateBulletNormal(Bullet& bullet, const float & elapsed);
	void UpdateBulletSimple(Bullet& bullet, const float & elapsed);
	void UpdateBulletAdvanced(Bullet& bullet, const float & elapsed);
	void UpdateBulletRealistic(Bullet& bullet, const float & elapsed);
	void CheckBulletCollisions();
};

