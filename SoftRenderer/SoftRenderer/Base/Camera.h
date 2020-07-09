#pragma once
#include "Base/Vector.hpp"
class Camera
{
public:
	Camera();
	Camera(Vector3f position, Vector3f upDirection, Vector3f forwardDirection);
	~Camera();

	void SetCameraUpDirection(Vector3f upDirection);
	void SetCameraForwardDirection(Vector3f forwardDirection);
	void SetCameraPosition(Vector3f position);

	void MoveFront(int dir);
	void MoveLeft(int dir);

	Vector3f GetCameraUpDirection();
	Vector3f GetCameraRightDirection();
	Vector3f GetCameraForwardDirection();
	Vector3f GetCameraPosition();

	void LookAt(Vector3f position, Vector3f upDirection, Vector3f forwardDirection);
private:
	Vector3f position;
	Vector3f upDirection;
	Vector3f forwardDirection;
	Vector3f right;
	float moveSpeed = 1.f;
};

