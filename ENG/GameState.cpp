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

void GameState::CreateBullet(Vector3 & const position)
{
	Bullet bullet{};
	bullet.SetPosition(position);
	bullets.emplace_back(bullet);
}
