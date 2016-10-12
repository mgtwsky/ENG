#pragma once

using namespace DirectX::SimpleMath;

class Hitbox
{
public:
	Hitbox();
	~Hitbox();
	bool Collides(Hitbox const & hitbox);
	Vector3							position;
};

