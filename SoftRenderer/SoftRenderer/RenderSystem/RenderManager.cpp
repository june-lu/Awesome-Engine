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
	renderContext->frameBuffer.resize(sdlInterface->screenWidth * sdlInterface->screenHeight);
	renderContext->depthBuffer.resize(sdlInterface->screenWidth * sdlInterface->screenHeight);

	std::fill(renderContext->frameBuffer.begin(), renderContext->frameBuffer.end(), std::numeric_limits<unsigned int>::max());
	std::fill(renderContext->depthBuffer.begin(), renderContext->depthBuffer.end(), std::numeric_limits<float>::infinity());

	rasterizer = new Rasterizer(renderContext);
}

RenderManager::~RenderManager()
{
}

void RenderManager::DrawTriangleByBarycentricCoordinates(Color color, Vector3f* pts, ShadedMode shadedmodel)
{
	rasterizer->DrawTriangleByBarycentricCoordinates(color, pts, shadedmodel);
}

void RenderManager::handleEvents()
{
	sdlInterface->handleEvents();
}

void RenderManager::SwapBuffer()
{
	sdlInterface->SwapBuffer(renderContext);
}

void RenderManager::RenderClear()
{

	sdlInterface->RenderClear(&Color::white);
}

void RenderManager::DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures, ShadedMode shadedMode)
{
	renderContext->shadedMode = shadedMode;
	renderContext->textures = textures;
	for (uint32_t i = 0; i < indices.size() / 3; i++)
	{
		Triangle triangle;
		for (int j = 0; j < 3; j++)
		{
			Vertex vertex = vertices[indices[3 * i + j]];

			triangle.SetVertex(j, vertex.position);
			triangle.SetNormal(j, vertex.normal);
			triangle.SetTexCoord(j, vertex.texCoords);
		}

		renderContext->triangles.push_back(triangle);
	}
	rasterizer->DrawTriangleByBarycentricCoordinates();
}