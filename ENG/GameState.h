#pragma once

#include "Wall.h"
#include "Bullet.h"

class GameState
{
public:
	GameState();
	~GameState();
	std::vector<Wall>			walls;
	std::vector<Bullet>			bullets;
};

