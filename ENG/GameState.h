#pragma once

#include "Wall.h"
#include "Bullet.h"
#include "Player.h"
#include "GameConstants.h"

enum class BallisticsType { NORMAL, SIMPLE, ADVANCED, REALISTIC };

class GameState
{
public:
	GameState();
	~GameState();
	void CreateBullet(Vector3& const position, Vector3& const direction = Vector3{});
	void UpdateBullets(float const & elapsed);
	void DestroyDeadBullets();
	BallisticsType				ballistics_type;
	GameConstants				constants;
	Player						player;
	std::vector<Wall>			walls;
	std::vector<Bullet>			bullets;
private:
	void UpdateBulletsNormal(const float & elapsed);
};

