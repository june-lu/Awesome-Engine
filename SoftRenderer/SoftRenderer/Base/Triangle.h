#pragma once
#include "Base/Vector.hpp"
#include "Base/Color.h"
#include "Base/Texture.h"
#include <array>

class Triangle
{
public:

	Triangle() {};
	~Triangle() {};

	Vector4f pos[3];//color at each vertex
	Color color[3];//texture u,v
	Vector3f normal[3];
	Vector2f texCoords[3];

	std::vector<Texture> textures;

	void SetVertex(int ind, Vector4f ver); /*set i-th vertex coordinates */
	void SetNormal(int ind, Vector3f n); /*set i-th vertex normal vector*/
	void SetColor(int ind, Color color); /*set i-th vertex color*/
	void SetTexCoord(int ind, Vector2f uv); /*set i-th vertex texture coordinate*/

	void SetVertexs(const std::array<Vector4f, 3>& vers);
	void SetNormals(const std::array<Vector3f, 3>& normals);
	void SetColors(const std::array<Color, 3>& colors);
	void SetTexCoords(const std::array<Vector2f, 3>& uvs);
	std::array<Vector4f, (size_t)3> toVector4() const;

};

