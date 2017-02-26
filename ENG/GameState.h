#pragma once

#include "Wall.h"
#include "Bullet.h"
#include "Player.h"
#include "GameConstants.h"
#include "Wind.h"
#include "Bot.h"
#include "StepTimer.h"
#include "FrameTimeLogger.h"

class GameState {
public:
	GameState();
	~GameState();
	void CreateBullet(Vector3& const position, Vector3& const direction, BallisticsType type = BallisticsType::NORMAL);
	void UpdateBullets(float const & elapsed);
	void DrawRoomHitboxes(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive* shape);
	void DrawBots(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive* shape) const;
	void UpdateBots(float const & elapsed);
	void DestroyDeadBullets();
	void SetBulletsSize(float const & size);
	void CreateMultiplePPLBots();
	void CreateHeavyLoadBots();
	void ClearBots();
	void DrawInfo(SpriteBatch* spriteBatch, SpriteFont* font, DX::StepTimer const & timer);
	void IncreaseBulletSize(float const & sizeToIncrease);
	void DecreaseBulletSize(float const & sizeToIncrease);
	void IncreaseBulletSpeed(float const & speedToIncrease);
	void DecreaseBulletSpeed(float const & speedToDecrease);
	void AddBot();
	void RemoveBot();
	bool IsPlayerInHeavyLoadRoom() const;
	bool IsPlayerInMultiplePPLRoom() const;
	bool IntersectsWithRoom(Hitbox const & room, Hitbox const & collider);
	GameConstants				constants;
	Player						player;
	BallisticsType				creation_bullet_type;
	FrameTimeLogger             logger;
	std::vector<Wall>			walls;
	std::vector<Bullet>			bullets;
	std::vector<Wind>           winds;
	std::vector<Bot>            bots;
private:
	void UpdateBulletNormal(Bullet& bullet, const float & elapsed);
	void CheckWindInfluence(Bullet & bullet, const float & elapsed);
	void UpdateBulletSimple(Bullet& bullet, const float & elapsed);
	void UpdateBulletAdvanced(Bullet& bullet, const float & elapsed);
	void UpdateBulletRealistic(Bullet& bullet, const float & elapsed);
	bool CheckBulletCollisionGaps(const Bullet& bullet, const Vector3& before_move, const Vector3& after_move) const;
	void CheckBulletsCollisions();
	bool CheckWallCollision(Hitbox const & hitbox) const;
	void CreateWinds();
	float						bullet_size;
	float						bullet_hitbox_size;
	Hitbox                      heavy_load_room{ { 40.f, 20.f, -10.f },{ 20.f, 23.f, 30.f } };
	Hitbox                      multiple_ppl_room{ { -40.f, 20.f, -10.f },{ 20.f, 23.f, 30.f } };
	const XMVECTORF32           room_hitbox_color{ 1.f,1.f,0.f,0.2f };
};

