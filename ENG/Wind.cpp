#include "pch.h"
#include "Wind.h"


Wind::Wind(Vector3 const & position, Vector3 const & size, Vector3 const & direction) : position{ position }, direction{ direction }, hitbox{ position, size / 2 } {}

Wind::Wind() {}


Wind::~Wind() {}

bool Wind::Contains(Hitbox const & hitbox) {
	return this->hitbox.Collides(hitbox);
}
