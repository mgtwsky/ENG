#include "pch.h"
#include "Bullet.h"


Bullet::Bullet() : destroyed{false}
{
}


Bullet::~Bullet()
{
}

void Bullet::Update(float & const elapsed)
{
	// todo implement behavior.
}

void Bullet::Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive * shape)
{
	shape->Draw(matrix, view, proj);
}
