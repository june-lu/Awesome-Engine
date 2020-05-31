#pragma once
#include "Vector2.h"
#include "Color.h"

class Vertex
{
public:
	Vertex();
	~Vertex();
};


class Vertex2D
{
public:
	Vertex2D();
	~Vertex2D();

private:
	Vector2f position;
	Color color;

	Vertex2D(Vector2f _position, Color _color) :position(_position), color(_color) {}
	Vertex2D interpolate(const Vertex2D &vertex, float t) const
	{
		Vector2f p = Vector2f::Lerp(position, vertex.position, t);
		Color c = Color::Lerp(color, vertex.color, t);
		return Vertex2D(p, c);
	};
};
