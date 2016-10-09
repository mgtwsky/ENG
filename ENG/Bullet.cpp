#include "pch.h"
#include "Bullet.h"


Bullet::Bullet() : alive{1.f}, direction{}
{
}


Bullet::~Bullet()
{
}

void Bullet::Update(float & const elapsed)
{
	alive.Update(elapsed);
}

void Bullet::Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive * shape)
{
	shape->Draw(matrix, view, proj);
}
