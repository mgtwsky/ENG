#pragma once
#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet();
	~Bullet();
	void Update(float& const elapsed);
	void Render(Matrix& const view, Matrix& const proj, GeometricPrimitive* shape);
};

