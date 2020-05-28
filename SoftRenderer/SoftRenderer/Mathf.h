#pragma once

#include <cmath>
#include <cfloat>

class Mathf
{
public:
	Mathf();
	~Mathf();

	static float Pi;
	static float Rad2Deg;
	static float Deg2Rad;
	static float Infinity;
	static float NegativeInfinity;
	static float EPSILON;
	static inline float Clamp(float x, float min = 0, float max = 1)
	{
		return fmin(max, fmax(x, min));
	}
	static inline float Min(float a, float b)
	{
		return fmin(a, b);
	}

	static inline float Max(float a, float b)
	{
		return fmax(a, b);
	}

	static inline  float Acos(float theta)
	{
		return acos(theta);
	}

	static inline float Asin(float theta)
	{
		return asin(theta);
	}

	static inline float Sin(float theta)
	{
		return sin(theta);
	}

	static inline float Cos(float theta)
	{
		return cos(theta);
	}

	static inline float Tan(float theta)
	{
		return tan(theta);
	}

	static inline int Round(float f)
	{
		return round(f);
	}

	static float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	static inline float Abs(float a)
	{
		return std::abs(a);
	}

};

