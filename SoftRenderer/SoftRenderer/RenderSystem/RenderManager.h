#pragma once
#include <vector>
#include <SDL.h>
#include "Base/SDLInterface.h"
#include "Base/Vector2.h"
#include "RenderSystem/Rasterizer.h"
#include "Base/Vertex.h"
#include "Base/Texture.h"

class RenderManager
{
public:
	RenderManager();
	RenderManager(const char* windowName, int width, int height);
	~RenderManager();

	void SwapBuffer();
	void RenderClear();
	void handleEvents();
	void DrawTriangleByBarycentricCoordinates(Color* color, Vector3f* pts, ShaderMode shadermodel = ShaderMode::Shaded);
	void DrawTriangleByBarycentricCoordinates(std::vector<Texture> textures, Vertex* vertexs, ShaderMode shadermodel = ShaderMode::Shaded);
	void DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);

	SDLInterface* sdlInterface;
	RenderContext* renderContext;
	Rasterizer* rasterizer;
};

