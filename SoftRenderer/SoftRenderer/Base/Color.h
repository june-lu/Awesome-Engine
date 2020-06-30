#pragma once

#include <iostream>
#include "Math/Mathf.h"
#include "Vector.hpp"

class Color
{
public:
	float r;
	float g;
	float b;
	float a;

	uint8_t uintR;
	uint8_t uintG;
	uint8_t uintB;
	uint8_t uintA;

	static Color red;
	static Color white;
	static Color green;
	static Color blue;
	static Color black;

	Color(float r, float g, float b, float a);
	Color(const Color& color);
	Color(uint32_t c);
	Color();
	~Color();

	int GetUintR();
	int GetUintG();
	int GetUintB();
	int GetUintA();

	Color &operator /=(float num);
	Color operator /(float num);
	Color &operator *=(float num);
	Color operator *(float num);
	Color &operator +=(const Color &color);
	Color &operator -=(const Color &color);
	Color &operator *=(const Color &color);
	Vector3f ToVec3();

	friend std::ostream& operator<<(std::ostream& output, const Color& color)
	{
		output << "color : (" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")" << std::endl;
		return output;
	}

	static inline Color Lerp(const Color& start, const Color& end, float t)
	{
		float r = Mathf::Lerp(start.r, end.r, t);
		float g = Mathf::Lerp(start.g, end.g, t);
		float b = Mathf::Lerp(start.b, end.b, t);
		float a = Mathf::Lerp(start.a, end.a, t);

		return Color(r, g, b, a);
	};

};