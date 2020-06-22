#pragma once

#include <iostream>
#include <vector>
#include "Base/Texture.h"
#include "Base/Vertex.h"
#include "Base/Triangle.h"
#include "Math/Matrix.hpp"
#include "Base/Camera.h"

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

	Camera camera;
	Texture texture;
	ShadedMode shadedMode;
	std::vector < uint32_t> frameBuffer;
	std::vector < float > depthBuffer;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;
	std::vector<Triangle> triangles;

	Matrix4f model;
	Matrix4f view;
	Matrix4f projection;
	Matrix4f viewport;
};

