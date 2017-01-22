#pragma once
#include "Hitbox.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Entity {
public:
	Entity();
	~Entity();
	Vector3 GetPosition() const;
	void SetPosition(Vector3 const &);
	bool									is_alive;
	XMVECTORF32								color;
	Vector3									position;
	Vector3                                 size;
	Hitbox									hitbox;
};

