#include "pch.h"
#include "Hitbox.h"


Hitbox::Hitbox() : bound{} {}

Hitbox::Hitbox(Vector3 const & position, Vector3 const & extends) : bound{ position, extends } {}


Hitbox::~Hitbox() {}

bool Hitbox::Collides(Hitbox const & hitbox) const {
	return bound.Contains(hitbox.bound);
}

Vector3 Hitbox::GetPosition() const {
	return Vector3(bound.Center);
}

void Hitbox::SetPosition(Vector3 const & position) {
	bound.Center = position;
}

Vector3 Hitbox::GetExtends() const {
	return Vector3(bound.Extents);
}

void Hitbox::SetExtends(Vector3 const & extends) {
	bound.Extents = extends;
}
