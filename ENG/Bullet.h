#pragma once
#include "Entity.h"
#include "Counter.h"
#include "GameConstants.h"

class Bullet : public Entity {
public:
	Bullet();
	~Bullet();
	void Update(float const & elapsed);
	void Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive* shape, GeometricPrimitive* hitbox_shape);
	void SetBulletSize(float const & size);
	BallisticsType				ballistics_type;
	Counter						alive;
	Vector3						direction;
	XMVECTORF32                 hitbox_color;
};

