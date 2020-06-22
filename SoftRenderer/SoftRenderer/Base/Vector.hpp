#pragma once
#include <iostream>
#include <cassert>
#include <iomanip> 

template <typename T>
class Vector2;

template <typename T>
class Vector3;

template <typename T>
class Vector4;

template <typename T>
class Vector2
{
public:

	T x, y;

	Vector2() :x(0), y(0) {};
	Vector2(const T& _x, const T& _y) :x(_x), y(_y) {};
	Vector2(const Vector2<T> & v) :x(v.x), y(v.y) {};
	Vector2(const Vector3<T> & v) :x(v.x), y(v.y) {};
	~Vector2() {};

	Vector2<T>& operator=(const Vector2<T>& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	bool operator==(const Vector2<T> &v) const { return x == v.x_ && y == v.y; }
	bool operator!=(const Vector2<T> &v) const { return !operator==(v); }

	Vector2<T> operator+(const Vector2<T> &v) const {
		return Vector2<T>(x + v.x, y + v.y);
	}
	Vector2<T>& operator+=(const Vector2<T> &v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector2<T> operator-(const Vector2<T> &v) const {
		return Vector2<T>(x - v.x, y - v.y);
	}
	Vector2<T>& operator-=(const Vector2<T> &v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	template <typename U>
	Vector2<T> operator*(const U u) const {
		return Vector2<T>(x * u, y * u);
	}
	template <typename U>
	Vector2<T>& operator*=(const U u) {
		x *= u;
		y *= u;
		return *this;
	}
	template <typename U>
	Vector2<T> operator/(const U u) const {
		double inv = 1.0 / u;
		return Vector2<T>(x * inv, y * inv);
	}
	template <typename U>
	Vector2<T>& operator/=(const U u) {
		double inv = 1.0 / u;
		x *= inv;
		y *= inv;
		return *this;
	}

	Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

	const T& operator[](const int i) const {
		assert(i >= 0 && i < 2);
		if (i == 0) return x;
		return y;
	}
	T& operator[](const int i) {
		assert(i >= 0 && i < 2);
		if (i == 0) return x;
		return y;
	}

	friend std::ostream& operator<<(std::ostream &os, const Vector2<T> &v) {
		return os << v.x << " "
			<< std::setw(8) << v.y << std::endl;
	}

	static inline Vector2<T> Lerp(const Vector2<T> &v1, const Vector2<T> &v2, float factor)
	{
		return v1 + (v2 - v1) * factor;
	}
};

typedef Vector2<double>		Vector2d;
typedef Vector2<float>		Vector2f;
typedef Vector2<int> 	 	Vector2i;


template <typename T>
class Vector3
{
public:

	T x, y, z;
	static Vector3<T> zero;
	static Vector3<T> one;
	static Vector3<T> left;
	static Vector3<T> right;
	static Vector3<T> up;
	static Vector3<T> down;
	static Vector3<T> front;
	static Vector3<T> back;

	Vector3() :x(0), y(0) {};
	Vector3(const T& _x, const T& _y, const T& _z) :x(_x), y(_y), z(_z) {};
	Vector3(const Vector4<T> & v) :x(v.x), y(v.y), z(v.z) {};
	Vector3(const Vector3<T> & v) :x(v.x), y(v.y), z(v.z) {};
	Vector3(const Vector2<T> & v) :x(v.x), y(v.y), z(0) {};
	~Vector3() {};

	Vector3<T>& operator=(const Vector3<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	bool operator==(const Vector3<T> &v) const { return x == v.x_ && y == v.y && z == v.z; }
	bool operator!=(const Vector3<T> &v) const { return !operator==(v); }

	Vector3<T> operator+(const Vector3<T> &v) const {
		return Vector3<T>(x + v.x, y + v.y, z + v.z);
	}
	Vector3<T>& operator+=(const Vector3<T> &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vector3<T> operator-(const Vector3<T> &v) const {
		return Vector3<T>(x - v.x, y - v.y, z - v.z);
	}
	Vector3<T>& operator-=(const Vector3<T> &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3<T> operator* (const Vector3<T> &v) const {
		return Vector3<T>(x * v.x, y * v.y, z * v.z);
	}

	Vector3<T> operator * (const float u) const {
		return Vector3<T>(x * u, y * u, z * u);
	}
	Vector3<T>& operator*=(const float u) {
		x *= u;
		y *= u;
		z *= u;
		return *this;
	}
	template <typename U>
	Vector3<T> operator/(const U u) const {
		double inv = 1.0 / u;
		return Vector3<T>(x * inv, y * inv, z * inv);
	}
	template <typename U>
	Vector3<T>& operator/=(const U u) {
		double inv = 1.0 / u;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

	const T& operator[](const int i) const {
		assert(i >= 0 && i < 3);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	T& operator[](const int i) {
		assert(i >= 0 && i < 3);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	friend std::ostream& operator<<(std::ostream &os, const Vector3<T> &v) {
		return os << v.x << " "
			<< std::setw(8) << v.y << " "
			<< std::setw(8) << v.z << std::endl;
	}

	static inline Vector3<T> Lerp(const Vector3<T> &v1, const Vector3<T> &v2, float factor)
	{
		return v1 + (v2 - v1) * factor;
	}

	static Vector3<T> Barycentric(Vector3<T> *pts, Vector3<T> P)
	{
		Vector3<T> u = Cross(Vector3<T>(pts[2].x - pts[0].x, pts[1].x - pts[0].x,
			pts[0].x - P.x), Vector3<T>(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y));
		///* `pts` and `P` has integer value as coordinates
		//   so `abs(u[2])` < 1 means `u[2]` is 0, that means
		//   triangle is degenerate, in this case return something with negative coordinates */
		if (std::abs(u.z) < 1) return Vector3<T>(-1, 1, 1);
		return Vector3<T>(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	}
};

typedef Vector3<double> 			Vector3d;
typedef Vector3<float> 				Vector3f;
typedef Vector3<int> 					Vector3i;
typedef Vector3<unsigned int> Vector3u;

Vector3f Vector3f::zero(0, 0, 0);
Vector3f Vector3f::one(1, 1, 1);
Vector3f Vector3f::left(-1, 0, 0);
Vector3f Vector3f::right(1, 0, 0);
Vector3f Vector3f::up(0, 1, 0);
Vector3f Vector3f::down(0, -1, 0);
Vector3f Vector3f::front(0, 0, -1);
Vector3f Vector3f::back(0, 0, 1);

template <typename T>
inline Vector3<T> Cross(Vector3<T> lhs, Vector3<T> rhs)
{
	return Vector3<T>(lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x);
}

template <typename T>
inline Vector3<T> operator*(const float u, const Vector3<T> &v) {
	return v * u;
}



template <typename T>
class Vector4
{
public:

	T x, y, z, w;
	static Vector4<T> zero;
	static Vector4<T> one;
	static Vector4<T> left;
	static Vector4<T> right;
	static Vector4<T> up;
	static Vector4<T> down;

	Vector4() :x(0), y(0) {};
	Vector4(const T& _x, const T& _y, const T& _z, const T& _w) :x(_x), y(_y), z(_z), w(_w) {};
	Vector4(const Vector4<T> & v) :x(v.x), y(v.y), z(v.z), w(v.w) {};
	Vector4(const Vector3<T> & v) :x(v.x), y(v.y), z(v.z), w(1.0) {};
	~Vector4() {};

	Vector4<T>& operator=(const Vector4<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	bool operator==(const Vector4<T> &v) const { return x == v.x_ && y == v.y && z == v.z&& w == v.w; }
	bool operator!=(const Vector4<T> &v) const { return !operator==(v); }

	Vector4<T> operator+(const Vector4<T> &v) const {
		return Vector4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	Vector4<T>& operator+=(const Vector4<T> &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	Vector4<T> operator-(const Vector4<T> &v) const {
		return Vector4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	Vector4<T>& operator-=(const Vector4<T> &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	Vector4<T> operator*(const Vector4<T> &v) const {
		return Vector4<T>(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	template <typename U>
	Vector4<T> operator*(const U u) const {
		return Vector4<T>(x * u, y * u, z * u, w * u);
	}
	template <typename U>
	Vector4<T>& operator*=(const U u) {
		x *= u;
		y *= u;
		z *= u;
		w *= u;
		return *this;
	}
	template <typename U>
	Vector4<T> operator/(const U u) const {
		double inv = 1.0 / u;
		return Vector4<T>(x * inv, y * inv, z * inv, w * inv);
	}
	template <typename U>
	Vector4<T>& operator/=(const U u) {
		double inv = 1.0 / u;
		x *= inv;
		y *= inv;
		z *= inv;
		w *= inv;
		return *this;
	}

	Vector4<T> operator-() const { return Vector4<T>(-x, -y, -z, -w); }

	const T& operator[](const int i) const {
		assert(i >= 0 && i < 3);
		if (i == 0) return x;
		if (i == 1) return y;
		if (i == 2) return z;
		return w;
	}
	T& operator[](const int i) {
		assert(i >= 0 && i < 3);
		if (i == 0) return x;
		if (i == 1) return y;
		if (i == 2) return z;
		return w;
	}

	friend std::ostream& operator<<(std::ostream &os, const Vector4<T> &v) {
		return os << std::setw(8) << v.x << " "
			<< std::setw(8) << v.y << " "
			<< std::setw(8) << v.z << " "
			<< std::setw(8) << v.w << std::endl;
	}

	static inline Vector4<T> Lerp(const Vector4<T> &v1, const Vector4<T> &v2, float factor)
	{
		return v1 + (v2 - v1) * factor;
	}
};

typedef Vector4<double> Vector4d;
typedef Vector4<float> Vector4f;
typedef Vector4<int> Vector4i;
