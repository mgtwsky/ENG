#pragma once

#include "Entity.h"
#include "Hitbox.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct Wall : public Entity {
	Wall(ID3D11DeviceContext3* d3dcontext, Vector3& const size, Vector3& const position);
	Wall(ID3D11DeviceContext3* d3dcontext, Vector3& const size);
	Wall(ID3D11DeviceContext3* d3dcontext);
	~Wall();
	void Render(CXMMATRIX view, CXMMATRIX projection) const;
	static void CreateWalls(ID3D11DeviceContext3* d3dcontext, std::vector<Wall>& walls);
	static void CreateMainHall(ID3D11DeviceContext3 * d3dcontext, std::vector<Wall> & walls);
	static void CreateConnector(ID3D11DeviceContext3 * d3dcontext, std::vector<Wall> & walls);
	static void CreateHeavyLoadRoom(ID3D11DeviceContext3 * d3dcontext, std::vector<Wall> & walls);
	static void CreateMultiplePPLRoom(ID3D11DeviceContext3 * d3dcontext, std::vector<Wall> & walls);
	Vector3												size;
	std::shared_ptr<DirectX::GeometricPrimitive>		hitbox_shape;
	std::shared_ptr<DirectX::GeometricPrimitive>		shape;
};