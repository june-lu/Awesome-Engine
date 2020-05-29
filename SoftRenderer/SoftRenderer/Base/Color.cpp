#include "Color.h"

Color Color::red(1, 0, 0, 1);
Color Color::green(0, 1, 0, 1);
Color Color::blue(0, 0, 1, 1);
Color Color::black(0, 0, 0, 1);
Color Color::white(1, 1, 1, 1);

int Color::GetUintR() { return (int)r * 255; }
int Color::GetUintG() { return (int)g * 255; }
int Color::GetUintB() { return (int)b * 255; }
int Color::GetUintA() { return (int)a * 255; }

Color::Color()
{
	
}

Color::~Color()
{

}

Color::Color(uint32_t c)
{
	uint8_t uintR = c >> 16 & 0xFF;
	uint8_t uintG = c >> 8 & 0xFF;
	uint8_t uintB = c & 0xFF;
	uint8_t uintA = 255;

	float inv255 = 1.0 / 255;

	r = uintR * inv255;
	g = uintG * inv255;
	b = uintB * inv255;
	a = 1;
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
Color& Color::operator *=(float num)
{
	r *= num; b *= num; g *= num; a *= num;
	return *this;
}
Color& Color::operator +=(const Color &color)
{
	r += color.r; b += color.b; g += color.g; a += color.a;
	return *this;
}
Color& Color::operator -=(const Color &color)
{
	r -= color.r; b -= color.b; g -= color.g; a -= color.a;
	return *this;
}
Color& Color::operator *=(const Color &color)
{
	r *= color.r; b *= color.b; g *= color.g; a *= color.a;
	return *this;
}