#include "pch.h"
#include "Bullet.h"


Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::Update(float & const elapsed)
{
	// todo implement behavior.
}

void Bullet::Render(Matrix & const view, Matrix & const proj, GeometricPrimitive * shape)
{
	shape->Draw(matrix, view, proj);
}
