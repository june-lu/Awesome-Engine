#include "RenderManager.h"



RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

Matrix4f GetModelMatrix(Vector3f angle, Vector3f scale, Vector3f transform)
{
	Matrix4f model(Matrix4f::Identity);
	model = model.RotateX(angle.x);
	model = model.RotateY(angle.y);
	model = model.RotateZ(angle.z);
	model = model.scale(scale.x, scale.y, scale.z);
	model = model.Transform(transform);
	return model;
}

Matrix4f GetViewMatrix(Vector3f eyePos)
{
	Matrix4f view(Matrix4f::Identity);

	return view;
}

Matrix4f GetProjectionMatrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
	Matrix4f projection(Matrix4f::Identity);

	return projection;
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

	Vector3f angle(0, 150, 0);
	Vector3f scale(38, 38, 38);
	Vector3f transform = Vector3f::right * (renderContext->width / 2);
	Vector3f eye_pos = { 0, 0, 10 };

	rasterizer->set_model(GetModelMatrix(angle, scale, transform));
	rasterizer->set_view(GetViewMatrix(eye_pos));
	rasterizer->set_projection(GetProjectionMatrix(45.0, 1, 0.1, 50));
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