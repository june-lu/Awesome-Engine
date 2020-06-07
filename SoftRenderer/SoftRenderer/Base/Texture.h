#pragma once
#include <iostream>
#include <assimp/scene.h>
class Texture
{
public:
	Texture();
	~Texture();

	uint32_t id;
	std::string type;
	aiString path;
};

