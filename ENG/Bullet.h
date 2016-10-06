#pragma once
#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet();
	~Bullet();
	void Update(float& const elapsed);
	void Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive* shape);
};

