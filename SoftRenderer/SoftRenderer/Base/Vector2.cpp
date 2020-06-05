#include "Vector2.h"


Vector2f::Vector2f() {}
Vector2f::Vector2f(int _x, int _y)
{
	x = _x; y = _y; z = 0;
}
Vector2f::Vector2f(const Vector3f vec3)
{
	x = vec3.x;
	y = vec3.y;
}

Vector2f::~Vector2f() {}

