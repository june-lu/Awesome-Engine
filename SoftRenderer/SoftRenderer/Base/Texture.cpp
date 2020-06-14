#include "Texture.h"

vector<vector<Color>> Texture::textureArray;

Texture::Texture()
{
}


Texture::~Texture()
{
}


Color Texture::Sample(float inU, float inV)
{
	float u = inU - floor(inU);
	float v = inV - floor(inV);

	u = Mathf::Clamp(u, 0.f, 1.f);
	v = Mathf::Clamp(v, 0.f, 1.f);

	int u1 = (int)(std::round(u * (width - 1)));
	int v1 = (int)(std::round(v * (height - 1)));

	return textureArray[id][u1 + v1 * width];
}