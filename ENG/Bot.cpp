#include "pch.h"
#include "Bot.h"


Bot::Bot() {}

Bot::Bot(Vector3 const & position, Vector3 const & size, Vector3 const & look_direction, bool const & is_stationary) : position{ position }, size{ size }, look_direction{ look_direction }, hitbox{ position,size/2 }, is_stationary{ is_stationary } {}

Bot::~Bot() {}

void Bot::Update(float const & elapsed) {
	if (is_stationary) return;
	position += look_direction * 10.f * elapsed;
	hitbox.SetPosition(position);
}

void Bot::Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive * shape, GeometricPrimitive * hitbox_shape) const {
	Matrix bot_pos = Matrix::CreateScale(size) + Matrix::CreateTranslation(position);

	shape->Draw(bot_pos, view, proj);
}
