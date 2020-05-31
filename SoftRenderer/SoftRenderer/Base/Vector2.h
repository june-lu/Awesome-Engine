#pragma once
class Vector2f
{
public:
	Vector2f();
	Vector2f(int x, int y);
	~Vector2f();

	static Vector2f zero;
	static Vector2f one;
	static Vector2f left;
	static Vector2f right;
	static Vector2f up;
	static Vector2f down;

	float x;
	float y;

	Vector2f operator+(const Vector2f& vec) const;
	Vector2f operator+=(const Vector2f& vec);
	Vector2f operator-(const Vector2f& vec) const;
	Vector2f operator-=(const Vector2f& vec);
	Vector2f operator*=(const Vector2f& vec);

	friend Vector2f operator * (const Vector2f& lhs, double rhs);
	friend Vector2f operator * (double lhs, const Vector2f& rhs);

	static inline Vector2f Lerp(const Vector2f &v1, const Vector2f &v2, float factor)
	{
		return v1 + (v2 - v1) * factor;
	}
};
