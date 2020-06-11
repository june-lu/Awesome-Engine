#include "Vector3.h"

Vector3f Vector3f::zero(0, 0, 0);
Vector3f Vector3f::one(1, 1, 1);
Vector3f Vector3f::left(-1, 0, 0);
Vector3f Vector3f::right(1, 0, 0);
Vector3f Vector3f::up(0, 1, 0);
Vector3f Vector3f::down(0, -1, 0);

Vector3f::Vector3f() {}

Vector3f::Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vector3f::~Vector3f() {}

Vector3f Vector3f::Cross(Vector3f lhs, Vector3f rhs)
{
	return Vector3f(lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x);
}

float Vector3f::Dot(Vector3f lhs, Vector3f rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}


Vector3f Vector3f::Barycentric(Vector3f *pts, Vector3f P)
{
	Vector3f u = Cross(Vector3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x,
		pts[0].x - P.x), Vector3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y));
	/* `pts` and `P` has integer value as coordinates
	   so `abs(u[2])` < 1 means `u[2]` is 0, that means
	   triangle is degenerate, in this case return something with negative coordinates */
	if (std::abs(u.z) < 1) return Vector3f(-1, 1, 1);
	return Vector3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

Vector3f Vector3f::operator+=(const Vector3f& vec)
{
	return Vector3f(x + vec.x, y + vec.y, z + vec.z);
}

Vector3f Vector3f::operator-=(const Vector3f& vec)
{
	return Vector3f(x - vec.x, y - vec.y, z - vec.z);
}

Vector3f Vector3f::operator*=(const Vector3f& vec)
{
	return Vector3f(x * vec.x, y * vec.y, z * vec.z);
}

Vector3f Vector3f::operator+(const Vector3f& vec)const
{
	return Vector3f(x + vec.x, y + vec.y, z + vec.z);
}

Vector3f Vector3f::operator-(const Vector3f& vec)const
{
	return Vector3f(x - vec.x, y - vec.y, z - vec.z);
}

Vector3f Vector3f::operator*(const Vector3f& vec)const
{
	return Vector3f(x * vec.x, y * vec.y, z * vec.z);
}

Vector3f operator * (const Vector3f& lhs, double rhs)
{
	return Vector3f(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
Vector3f operator * (double lhs, const Vector3f& rhs)
{
	return Vector3f(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}
std::ostream& operator<<(std::ostream& out, const Vector3f& rhs)
{
	out << "(x = " << rhs.x << ", y = " << rhs.y << ", z = " << rhs.z << ")" << std::endl;
	return out;
}

Vector3f Vector3f::Normalize()
{
	return Vector3f(x / Magnitude(), y / Magnitude(), z / Magnitude());
}

inline float Vector3f::Magnitude() const
{
	return std::sqrt(x * x + y * y + z * z);
}
inline float Vector3f::SqrtMagnitude() const
{
	return (x * x + y * y + z * z);
}

Vector3f Vector3f::RotationY(int angle)
{
	float radian = (float)angle * Mathf::Pi / 180.0;
	return Vector3f(x * Mathf::Cos(radian) - z * Mathf::Sin(radian), y, x * Mathf::Sin(radian) - z * Mathf::Cos(radian));
}