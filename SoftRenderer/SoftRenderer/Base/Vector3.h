#pragma once

#include "Vector2.h"
#include "Mathf.h"

class Vector3f
{
public:
	Vector3f();
	Vector3f(float x, float y, float z);
	~Vector3f();

	float x;
	float y;
	float z;

	static Vector3f Cross(Vector3f lhs, Vector3f rhs);
	static Vector3f Barycentric(Vector2f *pts, Vector2f P);
};

