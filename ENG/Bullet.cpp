#include "pch.h"
#include "Bullet.h"


Bullet::Bullet() : alive{ 5.f }, direction{}, ballistics_type{ BallisticsType::NORMAL }
{
}

Bullet::~Bullet()
{
}

void Bullet::Update(float const & elapsed)
{
	alive.Update(elapsed);
}

void Bullet::Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive * shape, GeometricPrimitive * hitbox_shape)
{
	const XMVECTORF32 hitbox_color{ 1.f,0.f,0.f,0.25f };
	shape->Draw(matrix, view, proj);
	Matrix hitbox_pos = Matrix::Identity;
	const Vector3 hitbox_pos_vec = hitbox.GetPosition();
	const Vector3 hitbox_extends_vec = hitbox.GetExtends();
	hitbox_pos.m[3][0] = hitbox_pos_vec.x;
	hitbox_pos.m[3][1] = hitbox_pos_vec.y;
	hitbox_pos.m[3][2] = hitbox_pos_vec.z;
	hitbox_pos.m[0][0] = hitbox_extends_vec.x;
	hitbox_pos.m[1][1] = hitbox_extends_vec.y;
	hitbox_pos.m[2][2] = hitbox_extends_vec.z;
	hitbox_shape->Draw(hitbox_pos, view, proj, hitbox_color);
}

void Bullet::SetBulletSize(float const & size)
{
	matrix.m[0][0] = size;
	matrix.m[1][1] = size;
	matrix.m[2][2] = size;
}
