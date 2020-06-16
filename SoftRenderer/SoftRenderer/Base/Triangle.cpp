#include "Triangle.h"


void Triangle::SetVertex(int ind, Vector4f ver)
{
	pos[ind] = ver;
}
void Triangle::SetNormal(int ind, Vector3f n)
{
	normal[ind] = n;
}
void Triangle::SetColor(int ind, Color c)
{
	color[ind] = c;
}
void Triangle::SetTexCoord(int ind, Vector2f uv)
{
	texCoords[ind] = uv;
}

void Triangle::SetVertexs(const std::array<Vector4f, 3>& vers)
{
	pos[0] = vers[0];
	pos[1] = vers[1];
	pos[2] = vers[2];
}

void Triangle::SetNormals(const std::array<Vector3f, 3>& normals)
{
	normal[0] = normals[0];
	normal[1] = normals[1];
	normal[2] = normals[2];
}
void Triangle::SetColors(const std::array<Color, 3>& colors)
{
	color[0] = colors[0];
	color[1] = colors[1];
	color[2] = colors[2];
}
void Triangle::SetTexCoords(const std::array<Vector2f, 3>& uvs)
{
	texCoords[0] = uvs[0];
	texCoords[1] = uvs[1];
	texCoords[2] = uvs[2];
}

std::array<Vector4f, 3> Triangle::toVector4() const
{
	std::array<Vector4f, 3> result;
	for (int i = 0; i < 3; i++)
	{
		result[i] = Vector4f(pos[i].x, pos[i].y, pos[i].z, 1.0);
	}
	return result;
}

