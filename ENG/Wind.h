#pragma once
#include "Bullet.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct Wind {
	Wind(Vector3 const & position, Vector3 const & size, Vector3 const & direction);
	Wind();
	~Wind();
	bool Contains(Hitbox const& hitbox) const;
	void Affect(Bullet & bullet, float const & elapsed) const;
	void Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive * hitbox_shape) const;

	Vector3           position;
	Vector3           direction;
	Hitbox            hitbox;
private:
	const XMVECTORF32 hitbox_color{ 0.f,1.f,0.f,0.2f };
};

