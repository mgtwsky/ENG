#include "pch.h"
#include "Hitbox.h"


Hitbox::Hitbox() : position{}
{
}


Hitbox::~Hitbox()
{
}

bool Hitbox::Collides(Hitbox const & hitbox)
{
	return false;
}
