#include "pch.h"
#include "Entity.h"

Entity::Entity()
{
	color = Colors::White;
}

Entity::~Entity()
{
}
// Gets the position of the entity.
Vector3 Entity::GetPosition() const
{
	Vector3 position{ matrix.m[3][0], matrix.m[3][1], matrix.m[3][2] };
	return position;
}
// Sets the position of the entity.
void Entity::SetPosition(Vector3 & const position)
{
	matrix.m[3][0] = position.x;
	matrix.m[3][1] = position.y;
	matrix.m[3][2] = position.z;
}
