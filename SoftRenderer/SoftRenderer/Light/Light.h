#pragma once
#include "Base/Vector.hpp"
#include "Base/Color.h"

enum LightType
{
	Directional,
	Point,
	Area,
	Spot,
	TypeCount
};

class Light
{
public:
	Light();
	~Light();

	Vector3f position;
	Vector3f direction;

	Color color;
	float intensity;
	float range;
	float spotAngle;


	LightType lightType;


};

