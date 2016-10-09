#pragma once

using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera();
	~Camera();
	Vector3 CreateLookDirectionVec() const;
	float												pitch;
	float												yaw;
	Matrix												proj;
	Vector3												camera_pos;
};

