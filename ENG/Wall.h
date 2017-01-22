#pragma once

#include "Entity.h"
#include "Hitbox.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Wall : public Entity
{
public:
	Wall(ID3D11DeviceContext3* d3dcontext, Vector3& const size, Vector3& const position);
	Wall(ID3D11DeviceContext3* d3dcontext, Vector3& const size);
	Wall(ID3D11DeviceContext3* d3dcontext);
	~Wall();
	void Render(CXMMATRIX view, CXMMATRIX projection) const;
	static void CreateWalls(ID3D11DeviceContext3* d3dcontext, std::vector<Wall>& walls);
	Vector3												size;
	std::shared_ptr<DirectX::GeometricPrimitive>		hitbox_shape;
	std::shared_ptr<DirectX::GeometricPrimitive>		shape;
};