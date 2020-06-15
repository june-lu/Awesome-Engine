#pragma once
#include "Base/Vector.hpp"
class Shader
{
public:
	Shader();
	~Shader();
};

struct fragment_shader_payload
{
	fragment_shader_payload()
	{
		texture = nullptr;
	}

	fragment_shader_payload(const Vector3f& col, const Vector3f& nor, const Vector2f& tc, Texture* tex) :
		color(col), normal(nor), tex_coords(tc), texture(tex) {}


	Vector3f view_pos;
	Vector3f color;
	Vector3f normal;
	Vector2f tex_coords;
	Texture* texture;
};

struct vertex_shader_payload
{
	Vector3f position;
};


