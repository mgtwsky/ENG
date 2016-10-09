#include "pch.h"
#include "GameState.h"


GameState::GameState() : player{}
{
	walls		= std::vector<Wall>();
	bullets		= std::vector<Bullet>(128);
}


GameState::~GameState()
{
}

void GameState::CreateBullet(Vector3 & const position, Vector3 & const direction)
{
	Bullet bullet{};
	bullet.SetPosition(position);
	bullet.direction = direction;
	bullets.emplace_back(bullet);
}

void GameState::UpdateBullets(float elapsed)
{
	for (auto& bullet : bullets) {
		bullet.Update(elapsed);
	}
}

void GameState::DestroyDeadBullets()
{
	bullets.erase(
		remove_if(
			bullets.begin(), 
			bullets.end(), 
			[](const Bullet& o) {return o.alive.Passed(); }), 
		bullets.end());
}
