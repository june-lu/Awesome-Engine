#pragma once
#include <vector>
#include <SDL.h>
#include "Base/SDLInterface.h"
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
	void DrawTriangleByBarycentricCoordinates(Color color, Vector3f* pts, ShadedMode shadedMode = ShadedMode::Shaded);
	void DrawTriangleByBarycentricCoordinates(std::vector<Texture> textures, Vertex* vertexs, ShadedMode shadedMode = ShadedMode::Shaded);
	void DrawMesh(Shader* shader, std::vector<Vertex> vertices, std::vector<uint32_t> indices,
		std::vector<Texture> textures, ShadedMode shadedMode = ShadedMode::Shaded);

	void SetCamera(Camera& camera);

	SDLInterface* sdlInterface;
	RenderContext* renderContext;
	Rasterizer* rasterizer;
};

