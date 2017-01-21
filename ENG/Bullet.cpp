#include "pch.h"
#include "Bullet.h"


Bullet::Bullet() : alive{ 2.f }, direction{}, ballistics_type{ BallisticsType::NORMAL } {}

Bullet::~Bullet() {}

void Bullet::Update(float const & elapsed) {
	alive.Update(elapsed);
}

void Bullet::Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive * shape, GeometricPrimitive * hitbox_shape) {
	const XMVECTORF32 hitbox_color{ 1.f,0.f,0.f,0.25f };
	shape->Draw(matrix, view, proj);
	Matrix hitbox_pos = Matrix::CreateScale(hitbox.GetExtends()) * Matrix::CreateTranslation(hitbox.GetPosition());
	hitbox_shape->Draw(hitbox_pos, view, proj, hitbox_color);
}

void Bullet::SetBulletSize(float const & size) {
	matrix.m[0][0] = size;
	matrix.m[1][1] = size;
	matrix.m[2][2] = size;
}
