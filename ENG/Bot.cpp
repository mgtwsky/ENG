#include "pch.h"
#include "Bot.h"


Bot::Bot() {}

Bot::Bot(Vector3 const & position, Vector3 const & size, Vector3 const & look_direction) : position{ position }, size{ size }, look_direction{ look_direction }, hitbox{ position,size } {}

Bot::~Bot() {}

void Bot::Update(float const & elapsed, Player const & player) {}

void Bot::Render(CXMMATRIX view, CXMMATRIX proj, GeometricPrimitive * shape, GeometricPrimitive * hitbox_shape) const {
	Matrix bot_pos = Matrix::CreateScale(size) + Matrix::CreateTranslation(position);

	shape->Draw(bot_pos, view, proj);
}
