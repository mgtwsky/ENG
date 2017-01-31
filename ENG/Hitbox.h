#pragma once

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct Hitbox {
	Hitbox();
	Hitbox(Vector3 const & position, Vector3 const & extends);
	~Hitbox();
	bool Collides(Hitbox const & hitbox) const;
	Vector3 GetPosition() const;
	void SetPosition(Vector3 const & position);
	Vector3 GetExtends() const;
	void SetExtends(Vector3 const & extends);
private:
	BoundingBox						bound;
};

