#pragma once
#include "Vector2.h"
#include "Color.h"
#include <string>
class Vertex
{
public:
	

	Vector3f position;
	Vector3f normal;
	Vector2f texCoords;
	std::string diffuseTexName;
	Color color;

	Vertex(Vector2f _position, Color _color) :position(_position), color(_color) {}
	Vertex(Vector3f _position, Vector3f _normal, Vector2f _texCoords)
		:position(_position), normal(_normal), texCoords(_texCoords){}
	~Vertex();
	Vertex interpolate(const Vertex &vertex, float t) const
	{
		Vector2f p = Vector2f::Lerp(position, vertex.position, t);
		Color c = Color::Lerp(color, vertex.color, t);
		return Vertex(p, c);
	};
};
