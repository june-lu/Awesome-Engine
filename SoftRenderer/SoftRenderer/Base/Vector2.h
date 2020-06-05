#pragma once
#include "Vector3.h"

class Vector2f :public Vector3f
{
public:
	Vector2f();
	Vector2f(int x, int y);
	Vector2f(const Vector3f vec3);
	~Vector2f();
	
};
 