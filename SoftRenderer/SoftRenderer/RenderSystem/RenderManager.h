#pragma once
#include <SDL.h>
#include "Base/SDLInterface.h"
#include "Base/Vector2.h"
#include "RenderSystem/Rasterizer.h"

class RenderManager
{
public:
	RenderManager();
	RenderManager(const char* windowName, int width, int height);
	~RenderManager();

	void SwapBuffer();
	void handleEvents();
	void DrawTriangleByBarycentricCoordinates(Color* color, Vector3f* pts);

	SDLInterface* sdlInterface;
	RenderContext* renderContext;
	Rasterizer* rasterizer;
};

