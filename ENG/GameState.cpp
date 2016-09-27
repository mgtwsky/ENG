#include "pch.h"
#include "GameState.h"


GameState::GameState()
{
	walls		= std::vector<Wall>();
	bullets		= std::vector<Bullet>();
}


GameState::~GameState()
{
}
