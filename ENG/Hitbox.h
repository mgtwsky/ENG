#pragma once

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Hitbox
{
public:
	Hitbox();
	Hitbox(Vector3 const & position, Vector3 const & extends);
	~Hitbox();
	bool Collides(Hitbox const & hitbox);
	Vector3 GetPosition() const;
	void SetPosition(Vector3 const & position);
private:
	BoundingBox						bound;
};

