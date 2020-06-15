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
	renderContext->frameBuffer.resize(sdlInterface->screenWidth * sdlInterface->screenHeight * 4);
	renderContext->depthBuffer.resize(sdlInterface->screenWidth * sdlInterface->screenHeight);

	//memset(renderContext->frameBuffer, 0x8f, renderContext->width * renderContext->height * 4);
	std::fill(renderContext->frameBuffer.begin(), renderContext->frameBuffer.end(), std::numeric_limits<unsigned int>::max());
	//memset(renderContext->depthBuffer, 0x7f, renderContext->width * renderContext->height * 4);
	std::fill(renderContext->depthBuffer.begin(), renderContext->depthBuffer.end(), std::numeric_limits<float>::infinity());

	rasterizer = new Rasterizer(renderContext);
}

RenderManager::~RenderManager()
{
}

void RenderManager::DrawTriangleByBarycentricCoordinates(Color color, Vector3f* pts, ShaderMode shadermodel)
{
	rasterizer->DrawTriangleByBarycentricCoordinates(color, pts, shadermodel);
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

void RenderManager::DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
{
	Vector3f scale(35, 35, 35);
	Vector3f transform = Vector3f::right * (renderContext->width / 2) + Vector3f::up;
	float angle = 0;


	for (uint32_t i = 0; i < indices.size(); i += 3)
	{
		Vertex vertexs[3];

		Vertex vertex = vertices[indices[i]];
		Vertex vertex1 = vertices[indices[i + 1]];
		Vertex vertex2 = vertices[indices[i + 2]];

		vertexs[0] = vertex;
		vertexs[1] = vertex1;
		vertexs[2] = vertex2;

		DrawTriangleByBarycentricCoordinates(textures, vertexs, ShaderMode::Shaded);
	}
}

void RenderManager::DrawTriangleByBarycentricCoordinates(std::vector<Texture> textures, Vertex* vertexs, ShaderMode shadermodel)
{
	rasterizer->DrawTriangleByBarycentricCoordinates(textures, vertexs, shadermodel);
}