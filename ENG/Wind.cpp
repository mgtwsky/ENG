#include "pch.h"
#include "Wind.h"


Wind::Wind(Vector3 const & position, Vector3 const & size, Vector3 const & direction) : position{ position }, direction{ direction }, hitbox{ position, size / 2 } {}

Wind::Wind() {}

Wind::~Wind() {}

bool Wind::Contains(Hitbox const & hitbox) const {
	return this->hitbox.Collides(hitbox);
}

void Wind::Affect(Bullet & bullet, float const & elapsed) const {
	const Vector3 bullet_new_position = bullet.GetPosition() += direction * elapsed;
	bullet.SetPosition(bullet_new_position);
}

void Wind::Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive * hitbox_shape) const {
	const Matrix hitbox_pos = Matrix::CreateScale(hitbox.GetExtends() * 2) * Matrix::CreateTranslation(hitbox.GetPosition());

	hitbox_shape->Draw(hitbox_pos, view, proj, hitbox_color);
}
