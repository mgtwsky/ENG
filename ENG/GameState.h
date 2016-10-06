#pragma once

#include "Wall.h"
#include "Bullet.h"
#include "Player.h"

class GameState
{
public:
	GameState();
	~GameState();
	Player						player;
	std::vector<Wall>			walls;
	std::vector<Bullet>			bullets;
};

