#include "pch.h"
#include "Entity.h"

Entity::Entity() : is_alive{ true }, size{ 1.f } {
	color = Colors::White;
}

Entity::~Entity() {}
// Gets the position of the entity.
Vector3 Entity::GetPosition() const {
	return position;
}
// Sets the position of the entity.
void Entity::SetPosition(Vector3 const & position) {
	this->position = position;
	hitbox.SetPosition(position);
}
