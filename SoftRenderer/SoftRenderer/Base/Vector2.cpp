#include "Vector2.h"

Vector2f Vector2f::zero(0, 0);
Vector2f Vector2f::one(1, 1);
Vector2f Vector2f::left(-1, 0);
Vector2f Vector2f::right(1, 0);
Vector2f Vector2f::up(0, 1);
Vector2f Vector2f::down(0, -1);

Vector2f::Vector2f() {}
Vector2f::Vector2f(int _x, int _y):x(_x),y(_y){}
Vector2f::~Vector2f(){}

Vector2f Vector2f::operator+=(const Vector2f& vec)
{
	return Vector2f(x + vec.x, y + vec.y);
}

Vector2f Vector2f::operator-=(const Vector2f& vec)
{
	return Vector2f(x - vec.x, y - vec.y);
}

Vector2f Vector2f::operator*=(const Vector2f& vec)
{
	return Vector2f(x * vec.x, y * vec.y);
}

Vector2f Vector2f::operator+(const Vector2f& vec)const
{
	return Vector2f(x + vec.x, y + vec.y);
}

Vector2f Vector2f::operator-(const Vector2f& vec)const
{
	return Vector2f(x - vec.x, y - vec.y);
}

Vector2f operator * (const Vector2f& lhs, double rhs)
{
	return Vector2f(lhs.x * rhs, lhs.y * rhs);
}
Vector2f operator * (double lhs, const Vector2f& rhs)
{
	return Vector2f(lhs * rhs.x , lhs * rhs.y);
}