#include "Color.h"

Color Color::red(1, 0, 0, 1);
Color Color::green(0, 1, 0, 1);
Color Color::blue(0, 0, 1, 1);
Color Color::black(0, 0, 0, 1);
Color Color::white(1, 1, 1, 1);

int Color::GetUintR() { return (int)(Mathf::Min(255, r * 255)); }
int Color::GetUintG() { return (int)(Mathf::Min(255, g * 255)); }
int Color::GetUintB() { return (int)(Mathf::Min(255, b * 255)); }
int Color::GetUintA() { return (int)(Mathf::Min(255, a * 255)); }

Color::Color()
{

}

Color::~Color()
{

}

Color::Color(uint32_t c)
{
	uintA = c >> 24 & 0xFF;
	uintB = c >> 16 & 0xFF;
	uintG = c >> 8 & 0xFF;
	uintR = c & 0xFF;


	float inv255 = 1.0 / 255;

	r = uintR * inv255;
	g = uintG * inv255;
	b = uintB * inv255;
	a = uintA * inv255;
}

Color::Color(float r, float g, float b, float a = 1)
{
	this->r = r; this->g = g; this->b = b; this->a = a;
}
Color::Color(const Color& color)
{
	this->r = color.r; this->g = color.g; this->b = color.b; this->a = color.a;
}

Color& Color::operator /=(float num)
{
	r /= num; b /= num; g /= num; a /= num;
	return *this;
}
Color Color::operator /(float num)
{
	return Color(r / num, g / num, b / num, a / num);
}
Color& Color::operator *=(float num)
{
	r *= num; g *= num; b *= num;
	return *this;
}
Color Color::operator *(float num)
{
	return Color(r  * num, g  * num, b  * num, a  * num);
}
Color& Color::operator +=(const Color &color)
{
	r += color.r; g += color.g; b += color.b;  a += color.a;
	return *this;
}

Color Color::operator +(Color &color)
{
	return Color(r + color.r, g + color.g, b + color.b, a + color.a);
}

Color& Color::operator -=(const Color &color)
{
	r -= color.r;  g -= color.g; b -= color.b; a -= color.a;
	return *this;
}
Color& Color::operator *=(const Color &color)
{
	r *= color.r;  g *= color.g; b *= color.b; a *= color.a;
	return *this;
}

Vector3f Color::ToVec3()
{
	return Vector3f(r, g, b);
}

uint32_t Color::ToUint32()
{
	return (GetUintA() << 24) | (GetUintB() << 16) | (GetUintG() << 8) | (GetUintR() << 0);
}