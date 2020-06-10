#pragma once
#include <iostream>
#include <assimp/scene.h>
#include "Base/Color.h"
#include <vector>

using namespace std;

class Texture
{
public:
	Texture();
	~Texture();

	uint32_t id;
	string type;
	aiString path;

	int width;
	int height;

	static vector<vector<Color>> textureArray;
};

