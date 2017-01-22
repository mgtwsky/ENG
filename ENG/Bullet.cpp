#include "pch.h"
#include "Bullet.h"


Bullet::Bullet() : alive{ 2.f }, direction{}, ballistics_type{ BallisticsType::NORMAL }, hitbox_color{ 1.f,0.f,0.f,0.25f } {}

Bullet::~Bullet() {}

void Bullet::Update(float const & elapsed) {
	alive.Update(elapsed);
}

void Bullet::Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive * shape, GeometricPrimitive * hitbox_shape) const {
	const Matrix bullet_pos = Matrix::CreateScale(size) * Matrix::CreateTranslation(position);
	const Matrix hitbox_pos = Matrix::CreateScale(hitbox.GetExtends() * 2) * Matrix::CreateTranslation(hitbox.GetPosition());

	shape->Draw(bullet_pos, view, proj);
	hitbox_shape->Draw(hitbox_pos, view, proj, hitbox_color);
}

void Bullet::SetBulletSize(float const & size) {
	this->size = Vector3(size);
}
