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
