#pragma once

#include <iostream>
#include <vector>
#include "Base/Texture.h"
#include "Base/Vertex.h"
#include "Base/Triangle.h"

enum ShadedMode
{
	Shaded,
	Wireframe,
	ShadedWireframe
};

class RenderContext
{
public:
	RenderContext();
	~RenderContext();

	int width;
	int height;
	Texture texture;
	ShadedMode shadedMode;
	std::vector < uint32_t> frameBuffer;
	std::vector < float > depthBuffer;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;
	std::vector<Triangle> triangles;
};

