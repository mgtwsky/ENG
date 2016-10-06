#pragma once
class Player
{
public:
	Player();
	~Player();
	DirectX::SimpleMath::Vector3						position;
	DirectX::SimpleMath::Vector3						look_direction;
};

