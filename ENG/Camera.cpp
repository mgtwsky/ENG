#include "pch.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

Vector3 Camera::CreateLookDirectionVec() const
{
	float y = sinf(pitch);
	float r = cosf(pitch);
	float z = r*cosf(yaw);
	float x = r*sinf(yaw);
	return Vector3{ x, y, z };
}
