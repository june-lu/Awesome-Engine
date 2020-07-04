#pragma once
#include "Color.h"
#include "Vector.hpp"

class Vertex
{
public:
	

	Vector3f position;
	Vector3f normal;
	Vector2f texCoords;
	Color color;

	Vertex(Vector2f _position, Color _color) :position(_position), color(_color) {}
	Vertex(Vector3f _position, Vector3f _normal, Vector2f _texCoords)
		:position(_position), normal(_normal), texCoords(_texCoords),color(Color::white){}
	Vertex();
	~Vertex();
	Vertex interpolate(const Vertex &vertex, float t) const
	{
		Vector2f p = Vector2f::Lerp(position, vertex.position, t);
		Color c = Color::Lerp(color, vertex.color, t);
		return Vertex(p, c);
	};
};
