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

void RenderManager::DrawTriangleByBarycentricCoordinates(Color* color, Vector3f* pts, ShaderMode shadermodel)
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
void RenderManager::DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	Vector3f scale(-35, 35, 35);
	Vector3f transform = Vector3f::right * (renderContext->width / 2);
	float angle = 0;
	std::cout << "indices.size(): " << indices.size() << std::endl;
	for (uint32_t i = 0; i < indices.size(); i += 3)
	{

		//std::cout << "vertices[indices[i + 1]].position : " << vertices[indices[i + 1]].position << std::endl;
		//std::cout << "vertices[indices[i + 2]].position : " << vertices[indices[i + 2]].position << std::endl;

		/*std::cout << "i : " << i << std::endl;
		std::cout << "i + 1 : " << i + 1 << std::endl;
		std::cout << "i + 2: " << i + 2 << std::endl;*/
		
		

		Vector3f normal = vertices[indices[i]].normal.Normalize();
		Vector3f t[3] = { Vector3f::up * renderContext->height - vertices[indices[i]].position.RotationY(angle) * scale + transform,
			Vector3f::up * renderContext->height - vertices[indices[i + 1]].position.RotationY(angle) * scale + transform,
			Vector3f::up * renderContext->height - vertices[indices[i + 2]].position.RotationY(angle) * scale + transform };
		
		float dotResult = Vector3f::Dot(normal, Vector3f(1, 0, 0));
		if (dotResult <= 0) dotResult = 0;
		dotResult += 0.1;
		Color color(normal.x < 0?0:normal.x, normal.y < 0 ? 0 : normal.y, normal.z < 0 ? 0 : normal.z, 1.0);
		//std::cout << "dotResult : " << dotResult << std::endl;
		/*std::cout << "color.r : " << color.r << std::endl;
		std::cout << "color.g : " << color.g << std::endl;
		std::cout << "color.b : " << color.b << std::endl;*/
		DrawTriangleByBarycentricCoordinates(&color, t, ShaderMode::Shaded);
	}
}