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
	memset(renderContext->depthBuffer, 0x4f, renderContext->width * renderContext->height * 4);


	rasterizer = new Rasterizer(renderContext);
}

RenderManager::~RenderManager()
{
}

void RenderManager::DrawTriangleByBarycentricCoordinates(Color* color, Vector3f* pts)
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
void RenderManager::DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	Vector3f scale(50, 50, 50);
	Vector3f transform = Vector3f::right * 500;
	for (uint32_t i = 0; i < indices.size(); i += 3)
	{

		//std::cout << "vertices[indices[i + 1]].position : " << vertices[indices[i + 1]].position << std::endl;
		//std::cout << "vertices[indices[i + 2]].position : " << vertices[indices[i + 2]].position << std::endl;

		Vector3f normal = vertices[indices[i]].normal;
		Vector3f t[3] = { vertices[indices[i]].position * scale + transform,   vertices[indices[i + 1]].position * scale + transform,  vertices[indices[i + 2]].position * scale + transform };
		Color color(1.0, 1.0, 0.0, 1.0);
		DrawTriangleByBarycentricCoordinates(&color, t);
	}
}