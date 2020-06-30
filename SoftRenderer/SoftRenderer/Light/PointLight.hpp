#pragma once
#include "Base/Vector.hpp"
#include "Base/Color.h"

class PointLight
{
public:
	PointLight() {}
	PointLight(Color _color, Vector3f _position) :color(_color), position(_position) {}
	~PointLight() {}

	void SetAttenuationPara(float _constant, float _linear, float _quadratic)
	{
		constant = _constant;
		linear = _linear;
		quadratic = _quadratic;
	}
	float GetAttenuation(Vector3f viewPos)
	{
		float distance = (position - viewPos).SqrtMagnitude();
		float attenuation = 1.0 / (constant + linear * distance +
			quadratic * (distance * distance));

		return attenuation;
	}


	Vector3f position;
	Color color;
private:
	float constant;
	float linear;
	float quadratic;
};