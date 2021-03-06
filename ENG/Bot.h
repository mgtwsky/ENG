#pragma once
#include "Hitbox.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

struct Bot {
	Bot();
	Bot(Vector3 const & position, Vector3 const & size, Vector3 const & look_direction, bool const & is_stationary);
	~Bot();
	void Update(float const & elapsed);
	void Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive* shape, GeometricPrimitive* hitbox_shape) const;

	Vector3                   position;
	Vector3                   look_direction;
	Vector3                   size;
	Hitbox					  hitbox;
	bool                      is_stationary;
};

