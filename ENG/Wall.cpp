#include "pch.h"
#include "Wall.h"

Wall::Wall(ID3D11DeviceContext3 * d3dcontext, Vector3 & const size, Vector3 & const position) {
	SetPosition(position);
	this->size = size;
	hitbox.SetExtends(size);
	shape = GeometricPrimitive::CreateBox(d3dcontext, this->size);
	hitbox_shape = GeometricPrimitive::CreateBox(d3dcontext, this->hitbox.GetExtends());
}

Wall::Wall(ID3D11DeviceContext3 * d3dcontext, Vector3 & const size) {
	this->size = size;
	hitbox.SetExtends(size);
	shape = GeometricPrimitive::CreateBox(d3dcontext, this->size);
	hitbox_shape = GeometricPrimitive::CreateBox(d3dcontext, this->hitbox.GetExtends());
}

Wall::Wall(ID3D11DeviceContext3* d3dcontext) {
	shape = GeometricPrimitive::CreateBox(d3dcontext, this->size);
	hitbox_shape = GeometricPrimitive::CreateBox(d3dcontext, this->hitbox.GetExtends());
}

Wall::~Wall() {}

void Wall::Render(CXMMATRIX view, CXMMATRIX projection) {
	const XMVECTORF32 shape_color{ 1.f,0.f,1.f,0.5f };
	const XMVECTORF32 hitbox_color{ 0.f,0.f,1.f,0.2f };
	shape->Draw(matrix, view, projection, shape_color);
	Matrix hitbox_pos = Matrix::CreateScale(hitbox.GetExtends()) * Matrix::CreateTranslation(hitbox.GetPosition());
	hitbox_shape->Draw(hitbox_pos, view, projection, hitbox_color);
}

void Wall::CreateWalls(ID3D11DeviceContext3* d3dcontext, std::vector<Wall>& walls) {
	Vector3 size{ 40.f, .1f, 40.f };					// floor settings.
	Wall floor{ d3dcontext, size, Vector3(0.f, -size.y, 0.f) };
	walls.push_back(floor);
	Vector3 front_size{ 40.f, 10.f, .5f };					// front wall settings.
	Wall front_wall{ d3dcontext, front_size, Vector3(0.f, front_size.y / 2, 20.f) };
	walls.push_back(front_wall);
	Vector3 left_size{ .5f, 10.f, 40.f };					// left wall settings.
	Wall left_wall{ d3dcontext, left_size, Vector3(-20.f, left_size.y / 2, 0.f) };
	walls.push_back(left_wall);
	Vector3 right_size{ .5f, 10.f, 40.f };					// right wall settings.
	Wall right_wall{ d3dcontext, right_size, Vector3(20.f, right_size.y / 2, 0.f) };
	walls.push_back(right_wall);
}
