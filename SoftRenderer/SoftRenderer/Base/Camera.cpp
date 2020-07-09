#include "Camera.h"

Camera::Camera()
{
	upDirection = Vector3f::up;
	forwardDirection = Vector3f::back;
	position = Vector3f::zero;
}

Camera::Camera(Vector3f position, Vector3f upDirection, Vector3f forwardDirection)
{
	this->upDirection = upDirection.Normalized();
	this->forwardDirection = (forwardDirection - position).Normalized();
	this->right = Cross(this->upDirection, this->forwardDirection);
	this->position = position;
}

Camera::~Camera()
{
}

void Camera::SetCameraUpDirection(Vector3f upDirection)
{
	this->upDirection = upDirection;
}

void Camera::SetCameraForwardDirection(Vector3f forwardDirection)
{
	this->forwardDirection = forwardDirection;
}

void Camera::SetCameraPosition(Vector3f position)
{
	this->position = position;
}

Vector3f Camera::GetCameraUpDirection()
{
	return upDirection;
}
Vector3f Camera::GetCameraForwardDirection()
{
	return forwardDirection ;
}
Vector3f Camera::GetCameraPosition()
{
	return position;
}

Vector3f Camera::GetCameraRightDirection()
{
	return right;
}

void Camera::LookAt(Vector3f position, Vector3f upDirection, Vector3f forwardDirection)
{
	this->upDirection = upDirection.Normalized();
	this->forwardDirection = (forwardDirection - position).Normalized();
	this->right = Cross(this->upDirection, this->forwardDirection);
	this->position = position;
}

void Camera::MoveFront(int dir)
{
	position += dir * moveSpeed * forwardDirection;
}

void Camera::MoveLeft(int dir)
{
	position -= dir * moveSpeed * right;
}