#pragma once
class Camera
{
public:
	Camera();
	~Camera();
	float												pitch;
	float												yaw;
	DirectX::SimpleMath::Matrix							proj;
	DirectX::SimpleMath::Vector3						camera_pos;
};

