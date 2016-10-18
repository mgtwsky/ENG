#include "pch.h"
#include "Hitbox.h"


Hitbox::Hitbox() : bound{}
{
}

Hitbox::Hitbox(Vector3 const & position, Vector3 const & extends) : bound{ position, extends }
{
}


Hitbox::~Hitbox()
{
}

bool Hitbox::Collides(Hitbox const & hitbox)
{
	return bound.Intersects(hitbox.bound) || bound.Contains(hitbox.bound);
}

Vector3 Hitbox::GetPosition() const
{
	return Vector3(bound.Center);
}

void Hitbox::SetPosition(Vector3 const & position)
{
	bound.Center = position;
}
