#pragma once

#include "Wall.h"
#include "Bullet.h"
#include "Player.h"

class GameState
{
public:
	GameState();
	~GameState();
	void CreateBullet(Vector3& const position);
	Player						player;
	std::vector<Wall>			walls;
	std::vector<Bullet>			bullets;
};

