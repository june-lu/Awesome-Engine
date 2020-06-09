#pragma once
#include <iostream>
#include <assimp/scene.h>
#include "Base/Color.h"
class Texture
{
public:
	Texture();
	~Texture();

	uint32_t id;
	std::string type;
	aiString path;

	int width;
	int height;
	Color* colors;
};

