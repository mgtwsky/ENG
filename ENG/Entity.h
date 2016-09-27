#pragma once

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Entity
{
public:
	Entity();
	~Entity();
	Vector3 GetPosition() const;
	void SetPosition(Vector3& const);
	XMVECTORF32								color;
	Matrix									matrix;
};

