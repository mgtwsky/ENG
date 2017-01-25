#include "pch.h"
#include "Wall.h"

Wall::Wall(ID3D11DeviceContext3 * d3dcontext, Vector3 & const size, Vector3 & const position) {
	SetPosition(position);
	this->size = size;
	hitbox.SetExtends(size);
	shape = GeometricPrimitive::CreateBox(d3dcontext, { 1.f, 1.f, 1.f });
	hitbox_shape = GeometricPrimitive::CreateBox(d3dcontext, { 1.f, 1.f, 1.f });
}

Wall::Wall(ID3D11DeviceContext3 * d3dcontext, Vector3 & const size) {
	this->size = size;
	hitbox.SetExtends(size);
	shape = GeometricPrimitive::CreateBox(d3dcontext, { 1.f, 1.f, 1.f });
	hitbox_shape = GeometricPrimitive::CreateBox(d3dcontext, { 1.f, 1.f, 1.f });
}

Wall::Wall(ID3D11DeviceContext3* d3dcontext) {
	shape = GeometricPrimitive::CreateBox(d3dcontext, { 1.f, 1.f, 1.f });
	hitbox_shape = GeometricPrimitive::CreateBox(d3dcontext, { 1.f, 1.f, 1.f });
}

Wall::~Wall() {}

void Wall::Render(CXMMATRIX view, CXMMATRIX projection) const {
	const XMVECTORF32 shape_color{ 1.f,0.f,1.f,0.5f };
	const XMVECTORF32 hitbox_color{ 0.f,0.f,1.f,0.2f };
	const Matrix hitbox_pos = Matrix::CreateScale(hitbox.GetExtends() * 2) * Matrix::CreateTranslation(hitbox.GetPosition());
	const Matrix wall_pos = Matrix::CreateScale(size) * Matrix::CreateTranslation(position);

	shape->Draw(wall_pos, view, projection, shape_color);
	hitbox_shape->Draw(hitbox_pos, view, projection, hitbox_color);
}

void Wall::CreateWalls(ID3D11DeviceContext3* d3dcontext, std::vector<Wall>& walls) {
	CreateMainHall(d3dcontext, walls);
	CreateConnector(d3dcontext, walls);
}

void Wall::CreateMainHall(ID3D11DeviceContext3 * d3dcontext, std::vector<Wall> & walls) {
	Vector3 size{ 400.f, .1f, 400.f };
	Vector3 front_size{ 40.f, 10.f, .5f };
	Vector3 left_size{ .5f, 10.f, 40.f };
	Vector3 right_size{ .5f, 10.f, 40.f };

	Wall floor{ d3dcontext, size, Vector3(0.f, 0.f, 0.f) };
	Wall front_wall{ d3dcontext, front_size, Vector3(0.f, front_size.y / 2, 20.f) };
	Wall left_wall{ d3dcontext, left_size, Vector3(-20.f, left_size.y / 2, 0.f) };
	Wall right_wall{ d3dcontext, right_size, Vector3(20.f, right_size.y / 2, 0.f) };

	walls.push_back(floor);
	walls.push_back(front_wall);
	walls.push_back(left_wall);
	walls.push_back(right_wall);
}

void Wall::CreateConnector(ID3D11DeviceContext3 * d3dcontext, std::vector<Wall>& walls) {
	Vector3 front_wall_size{ 120.f,10.f,0.5f };

	Wall front_wall{ d3dcontext, front_wall_size, Vector3(0.f,front_wall_size.y / 2, -40.f) };

	walls.push_back(front_wall);
}
