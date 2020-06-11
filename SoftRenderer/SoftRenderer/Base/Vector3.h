#pragma once

#include "Mathf.h"
#include <iostream>
#include <cmath>
class Vector3f
{
public:
	Vector3f();
	Vector3f(float x, float y, float z);
	~Vector3f();

	float x;
	float y;
	float z;

	static Vector3f zero;
	static Vector3f one;
	static Vector3f left;
	static Vector3f right;
	static Vector3f up;
	static Vector3f down;

	Vector3f operator+(const Vector3f& vec) const;
	Vector3f operator+=(const Vector3f& vec);
	Vector3f operator-(const Vector3f& vec) const;
	Vector3f operator-=(const Vector3f& vec);
	Vector3f operator*=(const Vector3f& vec);
	Vector3f operator*(const Vector3f& vec) const;

	friend Vector3f operator * (const Vector3f& lhs, double rhs);
	friend Vector3f operator * (double lhs, const Vector3f& rhs);
	friend std::ostream& operator<<(std::ostream& out, const Vector3f& rhs);

	//¹éÒ»»¯
	Vector3f Normalize();
	inline float Magnitude() const;
	inline float SqrtMagnitude() const;

	Vector3f RotationY(int angle);

	static inline Vector3f Lerp(const Vector3f &v1, const Vector3f &v2, float factor)
	{
		return v1 + (v2 - v1) * factor;
	}

	static Vector3f Cross(Vector3f lhs, Vector3f rhs);
	static float Dot(Vector3f lhs, Vector3f rhs);
	static Vector3f Barycentric(Vector3f *pts, Vector3f P);
};

