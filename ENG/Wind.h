#pragma once
#include "Hitbox.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct Wind {
	Wind(Vector3 const & position, Vector3 const & size, Vector3 const & direction);
	Wind();
	~Wind();
	bool Contains(Hitbox const& hitbox);

	Vector3           position;
	Vector3           direction;
	Hitbox            hitbox;
};

