#include "RenderManager.h"



RenderManager::RenderManager()
{
}

RenderManager::RenderManager(const char* _windowName, int _width, int _height)
{
	sdlInterface = new SDLInterface(_windowName, _width, _height);
	renderContext = new RenderContext();
	renderContext->width = sdlInterface->screenWidth;
	renderContext->height = sdlInterface->screenHeight;
	renderContext->frameBuffer = new uint32_t[sdlInterface->screenWidth * sdlInterface->screenHeight];
	renderContext->depthBuffer = new float[sdlInterface->screenWidth * sdlInterface->screenHeight];

	memset(renderContext->frameBuffer, 0, renderContext->width * renderContext->height * 4);
	memset(renderContext->depthBuffer, 0, renderContext->width * renderContext->height * 4);


	rasterizer = new Rasterizer(renderContext);
}

RenderManager::~RenderManager()
{
}

void RenderManager::DrawTriangleByBarycentricCoordinates(Color* color, Vector2f* pts)
{
	rasterizer->DrawTriangleByBarycentricCoordinates(color, pts);
}

void RenderManager::handleEvents()
{
	sdlInterface->handleEvents();
}

void RenderManager::SwapBuffer()
{
	sdlInterface->SwapBuffer(renderContext);
}