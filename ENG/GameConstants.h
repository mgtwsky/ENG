#pragma once

using namespace DirectX::SimpleMath;

enum class BallisticsType { NORMAL, SIMPLE, ADVANCED, REALISTIC };

struct GameConstants
{
	const float			bullet_normal_speed				= 100.f;
	const Vector3		gravity_vec						= { 0.f, -0.5f, 0.f };
};

