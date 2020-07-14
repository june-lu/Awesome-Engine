#pragma once

#include <iostream>
#include <vector>
#include "Base/Texture.h"
#include "Base/Vertex.h"
#include "Base/Triangle.h"
#include "Math/Matrix.hpp"
#include "Base/Camera.h"
#include "RenderSystem/Shader.hpp"

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
	std::vector < std::vector<Texture>> allTextures;
	ShadedMode shadedMode;
	std::vector < uint32_t> frameBuffer;
	std::vector < uint32_t> MASSFrameBuffer;
	std::vector < float > depthBuffer;
	std::vector < float > MASSDepthBuffer;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Vector3f> normals;
	std::vector<Triangle> triangles;

	Matrix4f model;
	Matrix4f view;
	Matrix4f projection;
	Matrix4f viewport;
	Shader* shader;
};

