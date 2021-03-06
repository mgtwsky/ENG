#pragma once
#include "Entity.h"
#include "Counter.h"
#include "GameConstants.h"

struct Bullet : public Entity {
	Bullet();
	~Bullet();
	void Update(float const & elapsed);
	void Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive* shape, GeometricPrimitive* hitbox_shape) const;
	void SetBulletSize(float const & size);
	BallisticsType				ballistics_type;
	Counter						alive;
	Vector3						direction;
	XMVECTORF32                 hitbox_color;
};

